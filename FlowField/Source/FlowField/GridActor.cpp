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
		integrationField[GetIntegrationKey(FIntVector2D(pos.X, pos.Y))].cost = 255;
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
	TArray<FCostKey> openList; // Open list of djikstra
	openList.Empty(); // Empty out list

	openList.Add(FCostKey(goalLocation,0)); // Add goal location to list

	// Iterate until all nodes had been analyzed
	while (openList.Num() != 0)
	{
		FCostKey currentNode = openList.Pop(); // Remove end node
		integrationField[GetIntegrationKey(currentNode.pos)].cost = currentNode.cost; // Add current node to integration fiels
		TArray<FIntVector2D> neighbors = GetNeighbors(currentNode.pos); // Get all neighbors of current node

		// Iterate through all neighbors
		for (int i = 0; i < neighbors.Num(); i++) 
		{
			// Get that cost of neighbor is greater than the current node's location. Also check that isnt a wall
			if (currentNode.cost < GetIntegrationCostAtPosition(neighbors[i]) && GetIntegrationCostAtPosition(neighbors[i]) != 255)
			{
				FCostKey newKey = FCostKey(neighbors[i], currentNode.cost + 1); // Make a new cost for this neighbor node
				openList.Insert(newKey, 0); // Add neighbor to open list
			}
		}
	}
}

// Generates the flow field based on goal and integration field
void AGridActor::GenerateFlowField() 
{
	for (int i = 0; i < integrationField.Num(); i++) 
	{
		int nearestCost = 100000; // Best cost of neighbors
		FIntVector2D base = integrationField[i].pos; // The current node
		FIntVector2D targetNeighbor; // The target node. I.E the neighbor with the best cost

		TArray<FIntVector2D> neighbors = GetNeighbors(base); // Find all neighbors
		
		// Iterate through all neighbors
		for (int j = 0; j < neighbors.Num(); j++) 
		{
			// Check that neighbor's cost is better than the current best cost and make sure it isnt a wall
			if (GetIntegrationCostAtPosition(neighbors[j]) < nearestCost && GetIntegrationCostAtPosition(neighbors[j]) != 255)
			{
				targetNeighbor = neighbors[j]; // Set nearest neighbor
				nearestCost = GetIntegrationCostAtPosition(neighbors[j]); // Get best cost from neighbor as well
			}
		}

		FVector2D direction = FVector2D(targetNeighbor.x - base.x, targetNeighbor.y - base.y); // Make directional vector
		direction.Normalize(); // Normalize to make it a true directional vector

		if (integrationField[i].cost == 0) 
		{
			flowField.Add(FFlowKey(base, FVector2D(0,0))); // Add direction to flow field
		}
		else 
		{
			flowField.Add(FFlowKey(base, direction)); // Add direction to flow field
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
	if (position.y < gridSize.y - 1)
	{
		neighbors.Add(FIntVector2D(position.x, position.y + 1));
	}
	if (position.y < gridSize.y - 1 && position.x < gridSize.x - 1) 
	{
		neighbors.Add(FIntVector2D(position.x + 1, position.y + 1));
	}
	if (position.y > 0 && position.x < gridSize.x - 1) 
	{
		neighbors.Add(FIntVector2D(position.x + 1, position.y - 1));
	}
	if (position.y < gridSize.y - 1 && position.x > 0) 
	{
		neighbors.Add(FIntVector2D(position.x - 1, position.y + 1));
	}
	if (position.y > 0 && position.x > 0) 
	{
		neighbors.Add(FIntVector2D(position.x - 1, position.y - 1));
	}

	return neighbors;
}

// Get the current cost at a given position in the integration field
int AGridActor::GetIntegrationCostAtPosition(FIntVector2D position) 
{
	// Go through entire integration field
	for (int i = 0; i < integrationField.Num(); i++) 
	{
		// Find if given position is equal to this position
		if (integrationField[i].pos.Equals(position)) 
		{
			return integrationField[i].cost; // return cost if positions match up
		}
	}

	return 0; // Return nothing if nothing is found
}

// Gets the current node's index at a given position in the integration field
int AGridActor::GetIntegrationKey(FIntVector2D position) 
{
	// Go through entire integration field
	for (int i = 0; i < integrationField.Num(); i++)
	{
		// Find if given position is equal to this position
		if (integrationField[i].pos.Equals(position))
		{
			return i; // return key if positions match up
		}
	}

	return 0; // Return nothing if nothing is found
}

// Gets the flow direction vector at a given position
FVector2D AGridActor::GetVectorAtPosition(FIntVector2D position) 
{
	// Go through entire flow field
	for (int i = 0; i < flowField.Num(); i++)
	{
		// Find if given position is equal to this position
		if (flowField[i].pos.Equals(position))
		{
			return flowField[i].direction; // Return direction if positions match up
		}
	}

	return FVector2D(0, 0); // Return a non-directional vector if nothing is found
}

// Resets all fields for the next tick
void AGridActor::ResetFields() 
{
	flowField.Empty(); // Empties out the flow field
	costField.Empty(); // Empties out cost field
	integrationField.Empty(); // Empties out the integration field

	// Initialize all fields based on grid size
	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			FCostKey newKey;
			FIntVector2D position;

			costField.Add(FCostKey(FIntVector2D(x, y), 1)); // Initialize Cost field
			integrationField.Add(FCostKey(FIntVector2D(x, y), 6558)); // Initialize Integration field
		}
	}
}