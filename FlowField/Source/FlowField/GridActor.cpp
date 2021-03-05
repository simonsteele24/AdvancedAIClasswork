// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"
#include "Wall.h"
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

			costField.Add(FCostKey(FIntVector2D(x, y), 1)); // Initialize Cost field
			integrationField.Add(FCostKey(FIntVector2D(x, y), 6558));
		}
	}
}

// Generates the cost field based on where walls are, will be called in tick
void AGridActor::GenerateCostField() 
{
	TArray<AActor*> results; // Represents results from get all actors search
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWall::StaticClass(), results); // Get all walls

	// Iterate through walls
	for (int i = 0; i < results.Num(); i++) 
	{
		// Get wall grid position
		AWall* wallActor = Cast<AWall>(results[i]);
		FVector2D pos = wallActor->GetGridPosition();
	}
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

// Generates the integration field based on goal
void AGridActor::GenerateIntegrationField() 
{
	TArray<FCostKey> openList;
	openList.Empty();

	openList.Add(FCostKey(goalLocation,0));

	while (openList.Num() != 0)
	{
		FCostKey currentNode = openList.Pop();
		TArray<FIntVector2D> neighbors = GetNeighbors(currentNode.pos);

		for (int i = 0; i < neighbors.Num(); i++) 
		{
		}
	}
}

// Gets all neighbors based on position
TArray<FIntVector2D> AGridActor::GetNeighbors(FIntVector2D position) 
{
	TArray<FIntVector2D> neighbors;
	neighbors.Empty();

	if (position.x > 0) 
	{
		neighbors.Add(FIntVector2D(position.x - 1, position.y));
	}
	if (position.x < gridSize.x - 1) 
	{
		neighbors.Add(FIntVector2D(position.x + 1, position.y));
	}
	if (position.y > 0)
	{
		neighbors.Add(FIntVector2D(position.x, position.y - 1));
	}
	if (position.x < gridSize.y - 1)
	{
		neighbors.Add(FIntVector2D(position.x, position.y + 1));
	}

	return neighbors;
}

// Get the current cost at a given position in the integration field
int AGridActor::GetIntegrationCostAtPosition(FIntVector2D position) 
{
	for (int i = 0; i < integrationField.Num(); i++) 
	{
		if (integrationField[i].pos.Equals(position)) 
		{
			return integrationField[i].cost;
		}
	}

	return 0;
}