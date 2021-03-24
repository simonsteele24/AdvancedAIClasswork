// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileTower.h"
#include "GridActor.h"
#include "Kismet/GameplayStatics.h"

void AMobileTower::UpdatePosition()
{
	if (Grid != nullptr) 
	{
		position = FIntVector2D(FMath::RoundToInt(GetActorLocation().X / Grid->distanceBetweenCells), FMath::RoundToInt(GetActorLocation().Y / Grid->distanceBetweenCells));
	}
	else 
	{
		// Get all grid actor in level
		TArray<AActor*> result;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), result);

		// check if grid is there to set grid
		if (result.Num() != 0)
		{
			Grid = Cast<AGridActor>(result[0]);
		}
	}
}