// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"
#include "Kismet/GameplayStatics.h"

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

// Initializes all fields, will be called in begin play
void AGridActor::GenerateGrid()
{
	costField.Empty(); // Empties out cost field


	// Initialize all fields based on grid size
	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			FCostKey newKey;
			FIntVector2D position;

			costField.Add(FCostKey(FIntVector2D(x, y), 0.0f)); // Initialize Cost field
		}
	}
}

// Generates the cost field based on where walls are, will be called in tick
void AGridActor::GenerateCostField()
{
}

// Displays out entire grid using actors
void AGridActor::DisplayGrid()
{
	FActorSpawnParameters spawnParams; // Spawn parameters
	FVector location = FVector(0, 0, 0); // Location of representation node
	FRotator rotation = FRotator(0, 0, 0); // Rotation of representation node


	// Spawn actors on grid based on grid size and distance between each cell
	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			location = FVector(x * distanceBetweenCells, y * distanceBetweenCells, 100); // Update position based on grid position
			GetWorld()->SpawnActor<AActor>(gridRepresentation, location, rotation, spawnParams); // Spawn actor
		}
	}
}


float AGridActor::GetCostAtLocation(FIntVector2D pos) 
{
	// Go through entire cost field
	for (int i = 0; i < costField.Num(); i++)
	{
		// Find if given position is equal to this position
		if (costField[i].pos.Equals(pos))
		{
			return costField[i].cost; // Return cost if positions match up
		}
	}

	return 0; // Return a cost of 0 if nothing is found
}

// Resets all fields for the next tick
void AGridActor::ResetFields()
{
	GenerateGrid();
}