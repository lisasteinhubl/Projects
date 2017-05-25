// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class MINISTRYOFMAYHEM_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<class ATargetPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> CharacterClass;

	void Spawn();

	int index = 0;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
