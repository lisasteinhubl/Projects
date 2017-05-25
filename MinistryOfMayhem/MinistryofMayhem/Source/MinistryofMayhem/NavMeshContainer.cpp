// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "NavMeshContainer.h"


// Sets default values
ANavMeshContainer::ANavMeshContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANavMeshContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavMeshContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

