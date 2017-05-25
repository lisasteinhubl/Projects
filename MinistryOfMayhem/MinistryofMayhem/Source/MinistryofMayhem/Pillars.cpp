// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "Pillars.h"


// Sets default values
APillars::APillars()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APillars::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APillars::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<class AActor*>& APillars::getPillarArray()
{
    return PillarArray;
}

