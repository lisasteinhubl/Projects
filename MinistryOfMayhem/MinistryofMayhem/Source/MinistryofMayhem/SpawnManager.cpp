// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "SpawnManager.h"
#include "Engine/TargetPoint.h"


// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < 6; i++)
	{
		Spawn();
	}
	
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::Spawn()
{
	if (SpawnPoints.Num() != 0)
	
	{
		//FMath::RandRange(0, SpawnPoints.Num() - 1);
		FVector tempLocation = SpawnPoints[index]->GetActorLocation();
		FRotator tempRotator = SpawnPoints[index]->GetActorRotation();
		index++;

		// Spawn an ACharacter of subclass CharacterClass, at specified position and rotation
		ACharacter* Char = GetWorld()->SpawnActor<ACharacter>(CharacterClass, tempLocation, tempRotator);
		if (Char)
		{
			// Spawn the AI controller for the character
			Char->SpawnDefaultController();
            
		}
        
	}
}
