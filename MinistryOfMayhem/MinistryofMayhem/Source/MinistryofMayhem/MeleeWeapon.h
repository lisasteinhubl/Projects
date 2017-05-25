// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class MINISTRYOFMAYHEM_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	USkeletalMeshComponent* MeleeWeaponMesh;

	class AMinistryofMayhemCharacter* MyOwner;

	void SetMyOwner(class AMinistryofMayhemCharacter* owner) { MyOwner = owner; }
	class AMinistryofMayhemCharacter* GetMyOwner() { return MyOwner; }

};
