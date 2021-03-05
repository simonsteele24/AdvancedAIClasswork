// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

USTRUCT(immutable, noexport, BlueprintType)
struct FIntVector2D
{
	FIntVector2D(){}

	FIntVector2D(int _x, int _y) 
	{
		x = _x;
		y = _y;
	}

	bool Equals(FIntVector2D otherPos) 
	{
		return otherPos.x == x && otherPos.y == y;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int y;
};

USTRUCT(immutable, noexport, BlueprintType)
struct FCostKey
{
	FCostKey(){}

	FCostKey(FIntVector2D _pos, int _cost) 
	{
		pos = _pos;
		cost = _cost;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FIntVector2D pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int cost;
};



UCLASS()
class FLOWFIELD_API AGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Public Functions */
	UFUNCTION(BlueprintCallable) void GenerateGrid();
	UFUNCTION(BlueprintCallable) void DisplayGrid();
	UFUNCTION(BlueprintCallable) void GenerateCostField();
	UFUNCTION(BlueprintCallable) void GenerateIntegrationField();


	/* Public Vars */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FIntVector2D gridSize; // The overal size of the grid (i.e 5x5 grid)
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distanceBetweenCells; // The distance between each node (100 between [0,0] and [0,1])
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> gridRepresentation; // A node to represent where a cell is
	UPROPERTY(EditAnywhere) FIntVector2D goalLocation; // Grid location of goal

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Private Functions */
	UFUNCTION() TArray<FIntVector2D> GetNeighbors(FIntVector2D position);
	UFUNCTION() int GetIntegrationCostAtPosition(FIntVector2D position);

	/* Private vars */
	UPROPERTY() TArray<FCostKey> costField; // A map to represent all obstacle values on the grid
	UPROPERTY() TArray<FCostKey> integrationField; // A map to represent all cost values on the grid
};
