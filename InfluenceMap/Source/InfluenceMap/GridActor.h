// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

USTRUCT(immutable, noexport, BlueprintType)
struct FIntVector2D
{
	FIntVector2D() {}

	FIntVector2D(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	FORCEINLINE bool operator==(const FIntVector2D& other) 
	{
		return other.x == x && other.y == y;
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
	FCostKey() {}

	FCostKey(FIntVector2D _pos, float _cost)
	{
		pos = _pos;
		cost = _cost;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FIntVector2D pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float cost;
};


UCLASS()
class INFLUENCEMAP_API AGridActor : public AActor
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
	UFUNCTION(BlueprintCallable) void GenerateMobileCostField();
	UFUNCTION(BlueprintCallable) void ResetFields();
	UFUNCTION(BlueprintCallable) void GenerateHeatMap();
	UFUNCTION(BlueprintCallable) float GetCostAtLocation(FIntVector2D pos);
	UFUNCTION(BlueprintCallable) float GetHeatAtLocation(FIntVector2D pos);
	UFUNCTION(BlueprintCallable) int GetDistanceBetweenTwoPositions(FIntVector2D a, FIntVector2D b);

	/* Public Vars */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FIntVector2D gridSize; // The overal size of the grid (i.e 5x5 grid)
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float distanceBetweenCells; // The distance between each node (100 between [0,0] and [0,1])
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> gridRepresentation; // A node to represent where a cell is

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Private vars */
	UPROPERTY() TArray<FCostKey> costField; // A map to represent all obstacle values on the grid
	UPROPERTY() TArray<FCostKey> mobileCostField; // A map to represent all mobile towers cost on the grid
	UPROPERTY() TArray<FCostKey> heatMap; // A map to represent all towers and enemies in game
	UPROPERTY() TArray<FIntVector2D> openList; // An array to represent an open list of all positions checked
	UPROPERTY() TArray<FIntVector2D> closedList; // An array to represent a closed list of all positions checked

	/* Private Functions */
	UFUNCTION() float CalculateInfluence(float MinDist, float MaxDist, float MaxVal, float Distance);
	UFUNCTION() void GenerateMobileCostHere(FIntVector2D Pos, class AMobileTower * tower);
	UFUNCTION() void GenerateHeatCostHere(FIntVector2D Pos, FIntVector2D EnemyPos, float MaxDist, float MaxVal);
	UFUNCTION() TArray<FIntVector2D> FindNearestNeighbors(FIntVector2D Pos, class AMobileTower* tower);
	UFUNCTION() TArray<FIntVector2D> FindNearestNeighborsHeat(FIntVector2D Pos, FIntVector2D EnemyPos, float MaxPresence);
	UFUNCTION() bool ClosedListContains(FIntVector2D Pos);
};