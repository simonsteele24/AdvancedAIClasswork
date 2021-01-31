// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UCLASS()
class OBSTACKEAVOIDANCE_API AAgent : public AActor
{
	GENERATED_BODY()
	
	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UStaticMeshComponent* agentBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UArrowComponent* directionArrow;


public:	
	// Sets default values for this actor's properties
	AAgent();

	// Functions
	UFUNCTION(BlueprintCallable) void MoveToLocation();
	UFUNCTION(BlueprintCallable) bool CheckIfLocationNeedsToBeUpdated();
	UFUNCTION(BlueprintCallable) void GenerateNewLocation();
	UFUNCTION(BlueprintCallable) void MoveIndicatorToTargetLocation();

	// Public vars
	UPROPERTY(EditAnywhere) FVector minCornerForPointGen;
	UPROPERTY(EditAnywhere) FVector maxCornerForPointGen;
	UPROPERTY(EditAnywhere) float distanceBeforeNewLocation;
	UPROPERTY(EditAnywhere) class ALocationIndicator* locationIndicator;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Floats
	UPROPERTY() float dt;

	// Vectors
	UPROPERTY() FVector locationToMoveTo;

	// Private Functions
	UFUNCTION() FVector Seek();

	FVector Position = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadOnly) FVector SteeringVelocity = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float MaxSpeed = 700.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float DragForce = -0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float SeekStrength = 900.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float SeekDecelerationDistance = 1200.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
