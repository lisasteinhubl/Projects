// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "Projectile.h"
#include "MinistryofMayhemCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHitGround(AActor* Actor)
{
	
	if (Actor->IsA<AMinistryofMayhemCharacter>())
	{
		AMinistryofMayhemCharacter* character = (AMinistryofMayhemCharacter*)Actor;
		character->TakeDamage(50.0f, FDamageEvent(), GetInstigatorController(), this);
	}
}

