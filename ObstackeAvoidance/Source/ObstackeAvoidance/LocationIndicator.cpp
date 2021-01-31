// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationIndicator.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALocationIndicator::ALocationIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	indicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Indicator"));
	indicator->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALocationIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALocationIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

