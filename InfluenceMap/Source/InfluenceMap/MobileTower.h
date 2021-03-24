// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "MobileTower.generated.h"

/**
 * 
 */
UCLASS()
class INFLUENCEMAP_API AMobileTower : public ATower
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable) void UpdatePosition();
};
