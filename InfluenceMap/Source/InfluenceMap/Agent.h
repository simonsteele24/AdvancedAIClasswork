// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Agent.generated.h"

UCLASS()
class INFLUENCEMAP_API AAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgent();

	UFUNCTION(BlueprintCallable) void UpdateToNextLocation();
	UFUNCTION(BlueprintCallable) bool CheckIfLocationNeedsToBeUpdated();
	UFUNCTION(BlueprintCallable) void MoveTowardLocation();
	UFUNCTION(BlueprintCallable) void ApplyDamage(float damageNum);
	UFUNCTION(BlueprintCallable) bool CheckIfAgentNeedsToBeDestroyed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Private Vars */
	UPROPERTY() int TargetIndex = -1; // Represents the target index in the spline point path
	UPROPERTY() FVector TargetLocation; // Represents the target location for the agent
	UPROPERTY() class AGridActor* GridActor; // Represents a reference to the grid

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Public Vars */
	UPROPERTY(BlueprintReadWrite) USplineComponent* SplineComponent; // Represents the draw out path using
	                                                                 // splines

	UPROPERTY(EditAnywhere) float DistanceBeforeSwitch; // Represents the distance required before the agent switches
	                                                    // to another target

	UPROPERTY(EditAnywhere) float AgentSpeed; // The overal speed of the agent

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float health; // The health of the agent

	UPROPERTY(EditAnywhere) float EnemyPresenceRadius; // Represents the radius of the agent's presence
	UPROPERTY(EditAnywhere) float EnemyPresenceMaxValue; // Represents the maximum value of the presence

};
