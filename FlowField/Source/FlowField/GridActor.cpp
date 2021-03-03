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
	for (int x = 0; x < gridSize.X; x++) 
	{
		for (int y = 0; y < gridSize.Y; y++) 
		{
			costField.Add(TTuple<FVector2D, int>(FVector2D(x, y), 1)); // Initialize Cost field
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

		costField.Remove(pos); // Remove old cost from cost field
		costField.Add(TTuple<FVector2D, int>(pos, 255)); // Add new cost to cost field based on wall
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