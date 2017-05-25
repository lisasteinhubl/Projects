// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "RifleWeapon.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "MinistryofMayhemCharacter.h"
#include "BaseEnemy.h"


// Sets default values
ARifleWeapon::ARifleWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));
	RootComponent = RifleMesh;
	FireRate = 0.1f;
	WeaponRange = 10000.0f;

}

// Called when the game starts or when spawned
void ARifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARifleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAudioComponent* ARifleWeapon::PlayWeaponSound(USoundCue* Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
	return AC;
}

void ARifleWeapon::OnStartFire()
{
	FireAC = PlayWeaponSound(FireLoopSound);
	MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, RifleMesh, TEXT("MuzzleFlashSocket"));
	MuzzlePSC->ActivateSystem();

	GetWorldTimerManager().SetTimer(WeaponTimer, this, &ARifleWeapon::WeaponTrace, FireRate, true);
}

void ARifleWeapon::OnStopFire()
{
	GetWorldTimerManager().ClearTimer(WeaponTimer);
	if (FireAC != nullptr)
	{
		FireAC->Stop();
		PlayWeaponSound(FireFinishSound);
		MuzzlePSC->DeactivateSystem();
	}

}


void ARifleWeapon::WeaponTrace() {
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
	static FName MuzzleSocket = FName(TEXT("MuzzleFlashSocket"));

	// Start from the muzzle's position
	FVector StartPos = RifleMesh->GetSocketLocation(MuzzleSocket);

	// Get forward vector of MyPawn
	FVector Forward = MyOwner->GetActorForwardVector();

	// Calculate end position
	FVector EndPos = FVector(StartPos.X + (WeaponRange * Forward.X), StartPos.Y + (WeaponRange * Forward.Y), StartPos.Z + (WeaponRange * Forward.Z));

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	// This fires the ray and checks against all objects w/ collision
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, EndPos, FCollisionObjectQueryParams::AllObjects, TraceParams);

	// Did this hit anything?
	if (Hit.bBlockingHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, Hit.ImpactPoint);
		ABaseEnemy* enemy = Cast<ABaseEnemy>(Hit.GetActor());
		if (enemy)
		{
			enemy->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
}
