// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "GridActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Initializes all values, called in begin play
void AWall::Initialize() 
{
	TArray<AActor*> results; // Represents all grid actors in world

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), results); // Find all grid actors in world

	// Check if there are in fact grid actors
	if (results.Num() != 0) 
	{
		grid = Cast<AGridActor>(results[0]); // Set grid actor to first grid actor found
	}
}

// Sets the grid position based on grid displacement
void AWall::SetGridPosition() 
{
	FVector2D newLocation = FVector2D(GetActorLocation().X, GetActorLocation().Y); // Set new location as 2D location in world
	
	newLocation /= grid->distanceBetweenCells; // Divide by displacement

	gridPosition = FVector2D(FMath::RoundToFloat(newLocation.X), FMath::RoundToFloat(newLocation.Y)); // set grid position as rounded value
}