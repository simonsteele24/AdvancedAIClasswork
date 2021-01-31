// Fill out your copyright notice in the Description page of Project Settings.

#include "Agent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	agentBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	agentBody->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();
	
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
	// steering
	SteeringVelocity += (SteeringVelocity * DragForce * dt);
	SteeringVelocity += (Seek() * SeekStrength * dt);

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


// Checks if agent is close enough to location to regenerate location
bool AAgent::CheckIfLocationNeedsToBeUpdated() 
{
	float distance = FVector::Distance(GetActorLocation(), locationToMoveTo);
	return distance <= distanceBeforeNewLocation;
}


// Makes random new location
void AAgent::GenerateNewLocation() 
{
	locationToMoveTo = FVector(FMath::FRandRange(minCornerForPointGen.X, maxCornerForPointGen.X),
		                       FMath::FRandRange(minCornerForPointGen.Y, maxCornerForPointGen.Y),
		                       GetActorLocation().Z);
}

FVector AAgent::Seek()
{
	FVector dir = locationToMoveTo - Position;
	float distance = dir.Size();
	dir.Z = 0; // consider only 2d plane
	dir.Normalize();

	const float maxSpeedDistance = 500.0f;
	float speedRatio = FMath::Clamp(distance / SeekDecelerationDistance, 0.0f, 1.0f);

	return dir * speedRatio;
}