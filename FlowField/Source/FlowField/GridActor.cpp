// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"

// Sets default values
AGridActor::AGridActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Generates the overal grid, will be called in begin play
void AGridActor::GenerateGrid() 
{
	gridVals.Empty(); // Empties out all grid vals


	// Initialize grid based on grid size
	for (int x = 0; x < gridSize.X; x++) 
	{
		for (int y = 0; y < gridSize.Y; y++) 
		{
			gridVals.Add(TTuple<FVector2D, int>(FVector2D(x, y), 999));
		}
	}
}

// Displays out entire grid using actors
void AGridActor::DisplayGrid() 
{
	FActorSpawnParameters spawnParams; // Spawn parameters
	FVector location = FVector(0, 0, 0); // Location of representation node
	FRotator rotation = FRotator(0, 0, 0); // Rotation of representation node


	// Spawn actors on grid based on grid size and distance between each cell
	for (int x = 0; x < gridSize.X; x++) 
	{
		for (int y = 0; y < gridSize.Y; y++) 
		{
			location = FVector(x * distanceBetweenCells, y * distanceBetweenCells, 100); // Update position based on grid position
			GetWorld()->SpawnActor<AActor>(gridRepresentation, location, rotation, spawnParams); // Spawn actor
		}
	}
}