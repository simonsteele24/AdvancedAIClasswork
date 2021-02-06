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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class USceneComponent* leftRaycast;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class USceneComponent* rightRaycast;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UCapsuleComponent* capsuleCollision;


public:	
	// Sets default values for this actor's properties
	AAgent();

	// Functions
	UFUNCTION(BlueprintCallable) void MoveToLocation();
	UFUNCTION(BlueprintCallable) bool ConeCheck();
	UFUNCTION(BlueprintCallable) void UpdateCollisionScore();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) class AWanderPointer* wanderPointer;

	UPROPERTY(BlueprintReadOnly) FVector SteeringVelocity = FVector(0, 0, 0);
	UPROPERTY(BlueprintReadOnly) int numOfCollisions = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Floats
	UPROPERTY() float dt;
	UPROPERTY() bool bObjectInWay;
	UPROPERTY() int numOfOverlaps = 0;
	UPROPERTY() float wanderOffset = 0.0f;
	UPROPERTY() float wanderRadius = 0.0f;
	UPROPERTY(BlueprintReadWrite) float wanderOrientation = 0.0f;
	UPROPERTY() float startTime = 0.0f;

	// Vectors
	UPROPERTY() FVector locationToMoveTo;

	// Private Functions
	UFUNCTION() FVector Wander();
	UFUNCTION() void ChangeWanderDirection();
	UFUNCTION() FVector Seek(FVector location);
	UFUNCTION() FVector Avoid();
	UFUNCTION() FVector Seperate();
	UFUNCTION() FVector AvoidAgents();
	UFUNCTION() FVector RotatePointAroundActor(float amountToRotate, float distanceOfPoint);

	FVector Position = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander") float wanderStrength = 900.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander") float wanderRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander") float circleOffset = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander") float timeOfWander = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float MaxSpeed = 700.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float DragForce = -0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float SeekStrength = 900.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering") float SeekDecelerationDistance = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seperation") float SeperationStrength = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seperation") float SeperationThreshold = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seperation") float DecayCoefficient = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionAvoidance") float coneThreshold = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionAvoidance") float coneDistance = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionAvoidance") float avoidStrength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionAvoidance") float avoidDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AgentAvoidance") float agentAvoidanceStrength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AgentAvoidance") float agentCollisionRadius = 100.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
