// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocationIndicator.generated.h"

UCLASS()
class OBSTACKEAVOIDANCE_API ALocationIndicator : public AActor
{
	GENERATED_BODY()

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UStaticMeshComponent* indicator;
	
public:	
	// Sets default values for this actor's properties
	ALocationIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
