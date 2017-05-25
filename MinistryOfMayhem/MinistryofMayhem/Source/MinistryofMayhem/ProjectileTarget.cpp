// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "ProjectileTarget.h"
#include "ProjectileTargetController.h"

// Sets default values
AProjectileTarget::AProjectileTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AProjectileTargetController::StaticClass();

}

// Called when the game starts or when spawned
void AProjectileTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

