// Fill out your copyright notice in the Description page of Project Settings.

#include "Agent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GridActor.h"
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

	// Collision detection raycast on left
	leftmostRaycast = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("LeftmostRaycast"));
	leftmostRaycast->SetupAttachment(agentBody);

	// Collision detection raycast on right
	rightmostRaycast = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RightmostRaycast"));
	rightmostRaycast->SetupAttachment(agentBody);

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
		startTime = GetGameTimeSinceCreation();
	}

	// steering
	FVector thirdDimensionLocation = GetActorLocation();
	FIntVector2D position = FIntVector2D(FMath::RoundToInt(thirdDimensionLocation.X / grid->distanceBetweenCells), FMath::RoundToInt(thirdDimensionLocation.Y / grid->distanceBetweenCells));
	FVector2D direction = grid->GetVectorAtPosition(position);

	SteeringVelocity += (SteeringVelocity * DragForce * dt);
	SteeringVelocity += (FVector(direction.X, direction.Y, 0) * SeekStrength * dt);
	SteeringVelocity += (Seperate() * SeperationStrength * dt);

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


// Seperation Behaviour
FVector AAgent::Seperate()
{
	TArray<AActor*> result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAgent::StaticClass(), result);

	FVector steer = FVector(0, 0, 0);

	for (int i = 0; i < result.Num(); i++)
	{
		float dist = FVector::Distance(result[i]->GetActorLocation(), GetActorLocation());
		if (dist < SeperationThreshold && result[i] != this)
		{
			float strength = (SeperationThreshold - dist);
			FVector direction = GetActorLocation() - result[i]->GetActorLocation();
			direction.Normalize();

			steer += direction * strength;
		}
	}

	return steer;
}


// Rotates point by given angle and distance around actor
FVector AAgent::RotatePointAroundActor(float amountToRotate, float distanceOfPoint)
{
	FRotator rotation = GetActorRotation() + FRotator(0, 0, amountToRotate);

	return GetActorLocation() + (UKismetMathLibrary::GetForwardVector(rotation) * distanceOfPoint);
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

	return FVector(0, 0, 0);
}

// Gets and sets the grid
void AAgent::InitializeGrid() 
{
	TArray<AActor*> result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), result);
	grid = Cast<AGridActor>(result[0]);
}