// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

UCLASS()
class FLOWFIELD_API AGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Public Functions */
	UFUNCTION(BlueprintCallable) void GenerateGrid();
	UFUNCTION(BlueprintCallable) void DisplayGrid();

	

	/* Public Vars */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector2D gridSize; // The overal size of the grid (i.e 5x5 grid)
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distanceBetweenCells; // The distance between each node (100 between [0,0] and [0,1])
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> gridRepresentation; // A node to represent where a cell is

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Private vars */
	UPROPERTY() TMap<FVector2D, int> gridVals; // A map to represent all values on the grid
};
