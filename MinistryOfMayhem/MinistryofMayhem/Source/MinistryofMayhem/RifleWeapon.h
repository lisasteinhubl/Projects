// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RifleWeapon.generated.h"

UCLASS()
class MINISTRYOFMAYHEM_API ARifleWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARifleWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	USkeletalMeshComponent* RifleMesh;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundCue* FireLoopSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundCue* FireFinishSound;
	UPROPERTY(Transient)
	class UAudioComponent* FireAC;

	UAudioComponent* PlayWeaponSound(USoundCue* Sound);

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UParticleSystem* MuzzleFX;

	UPROPERTY(Transient)
	class UParticleSystemComponent* MuzzlePSC;

	UPROPERTY(EditAnywhere)
	float FireRate;

	UPROPERTY(EditAnywhere)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = Damage)
	float Damage = 2.0f;

	

	FTimerHandle WeaponTimer;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnStartFire();
	void OnStopFire();
	void WeaponTrace();

	class AMinistryofMayhemCharacter* MyOwner;

	void SetMyOwner(class AMinistryofMayhemCharacter* owner) { MyOwner = owner; }
	class AMinistryofMayhemCharacter* GetMyOwner() { return MyOwner; }
	
	
};
