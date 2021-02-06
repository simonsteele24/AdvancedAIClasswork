// Fill out your copyright notice in the Description page of Project Settings.

#include "Agent.h"
#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "LocationIndicator.h"
#include "DrawDebugHelpers.h"
#include "WanderPointer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Agent body component
	agentBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	agentBody->SetupAttachment(RootComponent);

	// Arrow to show direction
	directionArrow = CreateAbstractDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	directionArrow->SetupAttachment(agentBody);

	// Collision detection raycast on left
	leftRaycast = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("LeftRaycast"));
	leftRaycast->SetupAttachment(agentBody);

	// Collision detection raycast on right
	rightRaycast = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RightRaycast"));
	rightRaycast->SetupAttachment(agentBody);

	// Capsule collision to check for collisions
	capsuleCollision = CreateAbstractDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	capsuleCollision->SetupAttachment(agentBody);
}

// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();
	Position = GetActorLocation();
	startTime = GetGameTimeSinceCreation();
	ChangeWanderDirection();
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	dt = DeltaTime;
}


// Moves actor to location using obstacle avoidance
void AAgent::MoveToLocation() 
{
	if (GetGameTimeSinceCreation() - startTime > timeOfWander) 
	{
		ChangeWanderDirection();
		startTime = GetGameTimeSinceCreation();
	}

	// steering
	SteeringVelocity += (SteeringVelocity * DragForce * dt);
	SteeringVelocity += (Wander() * wanderStrength * dt);
	SteeringVelocity += (AvoidAgents() * agentAvoidanceStrength * dt);

	Avoid();

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (AvoidAgents() * agentAvoidanceStrength), FColor::Yellow, false, -0.1f, (uint8)'\000', 10.0f);

	if (bObjectInWay) 
	{
		SteeringVelocity += (Avoid() * avoidStrength * dt);
	}

	if (bObjectInWay) 
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (Avoid() * avoidStrength), FColor::Blue, false, -0.1f, (uint8)'\000', 10.0f);
	}

	Avoid();

	// limit Speed
	if (SteeringVelocity.Size() > MaxSpeed)
	{
		SteeringVelocity = SteeringVelocity.GetSafeNormal() * MaxSpeed;
	}

	// movement
	Position += (SteeringVelocity * dt);
	SetActorLocation(Position, true);
	Position = GetActorLocation();

	// orientation 
	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Position, Position + SteeringVelocity);
	SetActorRotation(PlayerRot);
}


// Does the seek steering behavior
FVector AAgent::Seek(FVector location)
{
	FVector dir = location - Position;
	float distance = dir.Size();
	dir.Z = 0; // consider only 2d plane
	dir.Normalize();

	const float maxSpeedDistance = 500.0f;
	float speedRatio = FMath::Clamp(distance / SeekDecelerationDistance, 0.0f, 1.0f);

	return dir * speedRatio;
}


// Does the obstacle avoidance behavior
FVector AAgent::Avoid() 
{
	FHitResult hit;

	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * coneDistance), ECollisionChannel::ECC_Visibility);

	bObjectInWay = false;

	if (hit.bBlockingHit)
	{
		if (!Cast<AAgent>(hit.Actor) && !Cast<AWanderPointer>(hit.Actor)) 
		{
			bObjectInWay = true;
			return Seek(hit.ImpactPoint + (hit.ImpactNormal * avoidDistance));
		}
	}


	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + (leftRaycast->GetForwardVector() * coneDistance), ECollisionChannel::ECC_Visibility);

	if (hit.bBlockingHit)
	{
		if (!Cast<AAgent>(hit.Actor) && !Cast<AWanderPointer>(hit.Actor))
		{
			bObjectInWay = true;
			return Seek(hit.ImpactPoint + (hit.ImpactNormal * avoidDistance));
		}
	}

	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + (rightRaycast->GetForwardVector() * coneDistance), ECollisionChannel::ECC_Visibility);

	if (hit.bBlockingHit)
	{
		if (!Cast<AAgent>(hit.Actor) && !Cast<AWanderPointer>(hit.Actor))
		{
			bObjectInWay = true;
			return Seek(hit.ImpactPoint + (hit.ImpactNormal * avoidDistance));
		}
	}



	return FVector(0, 0, 0);
}


// Check if any objects are in range for avoidance
bool AAgent::ConeCheck() 
{
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * coneDistance), FColor::Green,false,-1.0f,(uint8)'\000',10.0f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), RotatePointAroundActor(coneThreshold, coneDistance), FColor::Green, false, -1.0f, (uint8)'\000', 10.0f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), RotatePointAroundActor(coneThreshold, -coneDistance), FColor::Green, false, -1.0f, (uint8)'\000', 10.0f);

	TArray <AActor*> result;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObstacle::StaticClass(), result);

	FVector distance = FVector(0, 0, 0);

	for (int i = 0; i < result.Num(); i++) 
	{
		distance = result[i]->GetActorLocation() - GetActorLocation();

		if (FVector::DotProduct(distance, GetActorForwardVector()) > UKismetMathLibrary::DegCos(coneThreshold)) 
		{
			if (FVector::Distance(result[i]->GetActorLocation(), GetActorLocation()) <= coneDistance) 
			{
				return true;
			}
		}
	}

	return false;
}


// Rotates point by given angle and distance around actor
FVector AAgent::RotatePointAroundActor(float amountToRotate, float distanceOfPoint) 
{
	FRotator rotation = GetActorRotation() + FRotator(0, 0, amountToRotate);

	return GetActorLocation() + (UKismetMathLibrary::GetForwardVector(rotation) * distanceOfPoint);
}


// Wander Steering Behavior
FVector AAgent::Wander() 
{
	return Seek(wanderPointer->GetActorLocation());
}



// Updated after certain time
void AAgent::ChangeWanderDirection() 
{
	wanderOrientation += UKismetMathLibrary::RandomFloatInRange(-1.0f, 1.0f) * wanderRate;
}


// Check if collision score needs to be updated based on the number of overlaps on capsule component
void AAgent::UpdateCollisionScore() 
{
	TArray<AActor*> result;
	TArray<AActor*> newResult;
	capsuleCollision->GetOverlappingActors(result);
	newResult.Empty();

	// Take out self from overlapping actor list
	for (int i = 0; i < result.Num(); i++) 
	{
		if (result[i] != this) 
		{
			newResult.Add(result[i]);
		}
	}


	if (numOfOverlaps != newResult.Num() && newResult.Num() > numOfOverlaps) 
	{
		numOfCollisions++;
	}

	numOfOverlaps = newResult.Num();
}


// Avoids all agents
FVector AAgent::AvoidAgents() 
{
	TArray<AActor*> result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAgent::StaticClass(), result);


	for (int i = 0; i < result.Num(); i++) 
	{
		if (result[i] != this) 
		{
			FVector direction = GetActorLocation() - result[i]->GetActorLocation();

			if (direction.Size() <= agentCollisionRadius) 
			{
				direction.Normalize();
				return direction;
			}
		}
	}

	return FVector(0,0,0);

	/*float shortestTime = 99999999999.0f;
	AAgent* firstTarget = nullptr;
	float firstMinSeperation = 0.0f;
	float firstDistance = 0.0f;
	FVector firstRelativePosition = FVector(0,0,0);
	FVector firstRelativeVelocity = FVector(0,0,0);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAgent::StaticClass(), result);

	for (int i = 0; i < result.Num(); i++) 
	{
		if (result[i] != this) 
		{
			FVector relativePosition =  GetActorLocation() - result[i]->GetActorLocation();
			FVector relativeVelocity = SteeringVelocity - Cast<AAgent>(result[i])->SteeringVelocity;
			float relativeSpeed = relativeVelocity.Size();
			float timeToCollision = ((FVector::DotProduct(relativePosition, relativeVelocity)) / (relativeSpeed * relativeSpeed));
			float distance = relativePosition.Size();
			float minSeperation = distance - relativeSpeed * timeToCollision;
			
			if (minSeperation > 2 * agentCollisionRadius)
			{
				continue;
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Time to collision: ") + FString::SanitizeFloat(timeToCollision));
			if (timeToCollision > 0 && timeToCollision < shortestTime)
			{
				shortestTime = timeToCollision;
				firstTarget = Cast<AAgent>(result[i]);
				firstMinSeperation = minSeperation;
				firstDistance = distance;
				firstRelativePosition = relativePosition;
				firstRelativeVelocity = relativeVelocity;
			}
		}
	}

	if (firstTarget == nullptr) 
	{
		return FVector(0, 0, 0);
	}

	FVector relativePos;

	if (firstMinSeperation <= 0 || firstDistance < 2 * agentCollisionRadius) 
	{
		relativePos = GetActorLocation() - firstTarget->GetActorLocation();
	}
	else 
	{
		relativePos = firstRelativePosition + firstRelativeVelocity * shortestTime;
	}

	relativePos.Normalize();

	return relativePos * 500.0f;*/
}