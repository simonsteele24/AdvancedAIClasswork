// Fill out your copyright notice in the Description page of Project Settings.

#include "Agent.h"
#include "Components/StaticMeshComponent.h"

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
	// Get direction to point
	FVector direction = locationToMoveTo - GetActorLocation();
	direction.Normalize();

	// Move to direction at given speed
	AddActorWorldOffset(direction * agentSpeed * dt);
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