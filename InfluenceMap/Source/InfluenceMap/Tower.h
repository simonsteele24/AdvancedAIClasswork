// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.h"
#include "Tower.generated.h"

UCLASS()
class INFLUENCEMAP_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Private Vars */
	UPROPERTY() class AGridActor* Grid;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Public Functions */
	UFUNCTION(BlueprintCallable) void InitializePosition();

	/* Public Vars */
	UPROPERTY(EditAnywhere) int MinDistance; // Represents minimum distance for tower to fire
	UPROPERTY(EditAnywhere) int MaxDistance; // Represents maximum distance that tower can fire on
	UPROPERTY(EditAnywhere) float MaxValue; // Represents the maximum value a tile can be from a tower
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float Damage; // Represents the Damage the tower can do
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FIntVector2D position;

};
