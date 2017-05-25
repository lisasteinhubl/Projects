// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NavMeshContainer.generated.h"

UCLASS()
class MINISTRYOFMAYHEM_API ANavMeshContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavMeshContainer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
		ANavMeshBoundsVolume* SmallMesh;
	UPROPERTY(EditAnywhere)
		ANavMeshBoundsVolume* BigMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ANavMeshBoundsVolume* GetSmallMesh() { return SmallMesh; }
	ANavMeshBoundsVolume* GetBigMesh() { return BigMesh; }
	
	
};
