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


public:	
	// Sets default values for this actor's properties
	AAgent();

	// Functions
	UFUNCTION(BlueprintCallable) void MoveToLocation();
	UFUNCTION(BlueprintCallable) bool CheckIfLocationNeedsToBeUpdated();
	UFUNCTION(BlueprintCallable) void GenerateNewLocation();

	// Public vars
	UPROPERTY(EditAnywhere) FVector minCornerForPointGen;
	UPROPERTY(EditAnywhere) FVector maxCornerForPointGen;
	UPROPERTY(EditAnywhere) float distanceBeforeNewLocation;
	UPROPERTY(EditAnywhere) float agentSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Floats
	UPROPERTY() float dt;

	// Vectors
	UPROPERTY() FVector locationToMoveTo;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
