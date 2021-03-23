// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"
#include "GridActor.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	// Get and set grid actor
	TArray<AActor*> result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), result);
	GridActor = Cast<AGridActor>(result[0]);

	Super::BeginPlay();
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Updates target location to the next spline point
void AAgent::UpdateToNextLocation() 
{
	TargetIndex++;
	TargetLocation = SplineComponent->GetLocationAtSplinePoint(TargetIndex, ESplineCoordinateSpace::World);
}

// Checks if the location needs to be updated to the next
bool AAgent::CheckIfLocationNeedsToBeUpdated() 
{
	// Find distance between the two points
	float distance = FVector::Dist(TargetLocation, GetActorLocation());

	// Return if it is within range and that it isn't the last point on the path
	return distance <= DistanceBeforeSwitch && TargetIndex != SplineComponent->GetNumberOfSplinePoints() - 1;
}

// Moves the agent towards the target location
void AAgent::MoveTowardLocation() 
{
	// Get the directional vector to the target point
	FVector direction = TargetLocation - GetActorLocation();
	direction.Normalize();

	// Add movement accordingly
	AddActorWorldOffset(direction * AgentSpeed * GetWorld()->GetDeltaSeconds());
}

// Applys damage to the enemy based on the influence map and damage type
void AAgent::ApplyDamage(float damageNum) 
{
	// Get grid position
	FIntVector2D pos = FIntVector2D(FMath::RoundToInt(GetActorLocation().X / GridActor->distanceBetweenCells), FMath::RoundToInt(GetActorLocation().Y / GridActor->distanceBetweenCells));

	// Subtract health by damage
	health -= damageNum * GridActor->GetCostAtLocation(pos);
}

// Checks if health is low enough to kill it off
bool AAgent::CheckIfAgentNeedsToBeDestroyed() 
{
	return health <= 0.0f;
}