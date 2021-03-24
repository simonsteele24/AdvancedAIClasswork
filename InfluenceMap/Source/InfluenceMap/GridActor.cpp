// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"
#include "Tower.h"
#include "MobileTower.h"
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
	mobileCostField.Empty(); // Empties out mobile cost field

	// Initialize all fields based on grid size
	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			FCostKey newKey;
			FIntVector2D position;

			costField.Add(FCostKey(FIntVector2D(x, y), 0.0f)); // Initialize Cost field
			mobileCostField.Add(FCostKey(FIntVector2D(x, y), 0.0f)); // Initialize Mobile Cost Field
		}
	}
}

// Generates the cost field based on where walls are, will be called in tick
void AGridActor::GenerateCostField()
{
	TArray<AActor*> result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), result);

	for (int i = 0; i < costField.Num(); i++) 
	{
		for (int j = 0; j < result.Num(); j++) 
		{
			if (Cast<AMobileTower>(result[j]) == nullptr) 
			{
				ATower* tower = Cast<ATower>(result[j]);
				float dist = GetDistanceBetweenTwoPositions(costField[i].pos, tower->position);
				float influence = CalculateInfluence(tower->MinDistance, tower->MaxDistance, tower->MaxValue,
					dist);
				costField[i].cost += influence;
			}
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
			return costField[i].cost + mobileCostField[i].cost; // Return cost if positions match up
		}
	}

	return 0; // Return a cost of 0 if nothing is found
}

// Resets all fields for the next tick
void AGridActor::ResetFields()
{
	GenerateGrid();
}

// Gets the non-diagonal distance between 2 grid nodes
int AGridActor::GetDistanceBetweenTwoPositions(FIntVector2D a, FIntVector2D b)
{
	// Get the differences between x and y
	int diffOfX = std::abs(b.x - a.x);
	int diffOfY = std::abs(b.y - a.y);

	int result = diffOfX + diffOfY;

	return result; // Return sum of the 2 differences
}

// Calculates the influence based on tower vals
float AGridActor::CalculateInfluence(float MinDist, float MaxDist, float MaxVal, float Distance) 
{

	// return 0 if distance is out of range
	if ((Distance > MaxDist && Distance != MaxDist) ||(Distance < MinDist && Distance != MinDist)) 
	{
		return 0.0f;
	}

	return MaxVal - (MaxVal * ((Distance - MinDist) / (MaxDist - MinDist)));
}

// Generates the cost field based on the mobile towers in game
void AGridActor::GenerateMobileCostField() 
{
	TArray<AActor*> result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMobileTower::StaticClass(), result);

	openList.Empty();
	closedList.Empty();

	for (int i = 0; i < mobileCostField.Num(); i++) 
	{
		mobileCostField[i].cost = 0;
	}

	for (int i = 0; i < result.Num(); i++) 
	{
		AMobileTower* tower = Cast<AMobileTower>(result[i]);

		openList.Add(tower->position);

		while (openList.Num() != 0)
		{
			FIntVector2D newPos = openList.Pop();
			GenerateMobileCostHere(newPos, tower);
			closedList.Add(newPos);
			openList.Append(FindNearestNeighbors(newPos, tower));
		}
	}
}

// Generates the based on where the mobile actor is
void AGridActor::GenerateMobileCostHere(FIntVector2D Pos, class AMobileTower * tower)
{
	float dist = GetDistanceBetweenTwoPositions(Pos, tower->position);
	float influence = CalculateInfluence(tower->MinDistance, tower->MaxDistance, tower->MaxValue,
		dist);
	
	if ((influence > tower->MaxValue && influence != tower->MaxValue) || (influence < 0 && influence != 0)) 
	{
		return;
	}

	for (int i = 0; i < mobileCostField.Num(); i++) 
	{
		if (mobileCostField[i].pos.Equals(Pos)) 
		{
			mobileCostField[i].cost = influence;
		}
	}
}

// Finds the nearest neighbors relative to the mobile tower being checked
TArray<FIntVector2D> AGridActor::FindNearestNeighbors(FIntVector2D Pos, class AMobileTower * tower)
{
	TArray<FIntVector2D> neigbors;
	neigbors.Empty();

	if (Pos.x != 0) 
	{
		if (!ClosedListContains(FIntVector2D(Pos.x - 1, Pos.y))) 
		{
			if (GetDistanceBetweenTwoPositions(FIntVector2D(Pos.x -1, Pos.y), tower->position) <= tower->MaxDistance)
			{
				neigbors.Add(FIntVector2D(Pos.x - 1, Pos.y));
			}
		}
	}

	if (Pos.y != 0)
	{
		if (!ClosedListContains(FIntVector2D(Pos.x, Pos.y - 1 )))
		{
			if (GetDistanceBetweenTwoPositions(FIntVector2D(Pos.x, Pos.y - 1), tower->position) <= tower->MaxDistance)
			{
				neigbors.Add(FIntVector2D(Pos.x, Pos.y - 1));
			}
		}
	}

	if (Pos.y != gridSize.y - 1)
	{
		if (!ClosedListContains(FIntVector2D(Pos.x, Pos.y + 1)))
		{
			if (GetDistanceBetweenTwoPositions(FIntVector2D(Pos.x, Pos.y + 1), tower->position) <= tower->MaxDistance)
			{
				neigbors.Add(FIntVector2D(Pos.x, Pos.y + 1));
			}
		}
	}

	if (Pos.x != gridSize.x - 1)
	{
		if (!ClosedListContains(FIntVector2D(Pos.x + 1, Pos.y)))
		{
			if (GetDistanceBetweenTwoPositions(FIntVector2D(Pos.x + 1, Pos.y), tower->position) <= tower->MaxDistance)
			{
				neigbors.Add(FIntVector2D(Pos.x + 1, Pos.y));
			}
		}
	}

	return neigbors;
}

// Checks if a given position if in the open list
bool AGridActor::ClosedListContains(FIntVector2D Pos) 
{
	for (int i = 0; i < closedList.Num(); i++) 
	{
		if (closedList[i] == Pos) 
		{
			return true;
		}
	}

	return false;
}