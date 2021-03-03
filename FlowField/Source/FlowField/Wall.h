// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

UCLASS()
class FLOWFIELD_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Public Functions */
	UFUNCTION(BlueprintCallable) void Initialize();
	UFUNCTION(BlueprintCallable) void SetGridPosition();

	/* Non-blueprint public functions */
	UFUNCTION() FVector2D GetGridPosition() { return gridPosition; } // Returns grid position of the wall

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Private Vars */
	UPROPERTY() class AGridActor * grid; // Represents grid actor in world

	/* Private Vars */
	UPROPERTY() FVector2D gridPosition; // Represents actor position relative to grid
};
