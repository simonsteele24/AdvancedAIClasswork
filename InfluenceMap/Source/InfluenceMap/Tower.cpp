// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "GridActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATower::ATower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{

	Super::BeginPlay();
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATower::InitializePosition() 
{
	// Get all grid actor in level
	TArray<AActor*> result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), result);

	// check if grid is there to set grid
	if (result.Num() != 0)
	{
		Grid = Cast<AGridActor>(result[0]);
	}

	position = FIntVector2D(FMath::RoundToInt(GetActorLocation().X / Grid->distanceBetweenCells), FMath::RoundToInt(GetActorLocation().Y / Grid->distanceBetweenCells));
}