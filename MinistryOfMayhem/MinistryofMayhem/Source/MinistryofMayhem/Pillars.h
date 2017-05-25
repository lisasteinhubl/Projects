// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pillars.generated.h"

UCLASS()
class MINISTRYOFMAYHEM_API APillars : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillars();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Pillar)
		TArray<AActor*> PillarArray;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    TArray<AActor*>& getPillarArray();
	
};
