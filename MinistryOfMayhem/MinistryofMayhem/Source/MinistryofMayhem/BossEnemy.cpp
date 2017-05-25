// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "BossEnemy.h"
#include "Projectile.h"
#include "ProjectileTarget.h"
#include "ProjectileTargetController.h"
#include "MeleeEnemy.h"
#include "BossHelperController.h"

ABossEnemy::ABossEnemy()
{
	Health = 500.0f;
}

void ABossEnemy::Tick(float DeltaTime) {
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TrackTimer, this, &ABossEnemy::StartTrackingPlayer,
		30.0, true);
}

void ABossEnemy::StartTrackingPlayer()
{
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if (player0 && ProjectileTargetClass)
	{
		FVector projectileTargetLocation = player0->GetActorLocation();
		//FVector projectileTargetLocation = GetActorLocation();
		projectileTargetLocation.Z = -59.0;
		FRotator tempRotator = player0->GetActorRotation();

		ProjTarg = (AProjectileTarget*) GetWorld()->SpawnActor<APawn>(ProjectileTargetClass, projectileTargetLocation, tempRotator);
		if (ProjTarg)
		{
			// Spawn the AI controller for the character
			ProjTarg->SpawnDefaultController();
		}
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ABossEnemy::ThrowProjectile,
			5.0, false);
	}
}

void ABossEnemy::ThrowProjectile()
{
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if (player0 && ProjectileClass && ProjTarg)
	{
		AProjectileTargetController* ProjController = (AProjectileTargetController*) ProjTarg->GetController();
		if (ProjController)
		{
			ProjController->HoldPosition();
		}

		if (JumpAnim)
		{
			GetMesh()->PlayAnimation(JumpAnim, false);
		}

		FVector projectileLocation = ProjTarg->GetActorLocation();
		projectileLocation.Z += 1000;
		FRotator tempRotator = ProjTarg->GetActorRotation();

		GetWorld()->SpawnActor<AActor>(ProjectileClass, projectileLocation, tempRotator);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ABossEnemy::DestroyProjectileTarget,
			5.0, false);
	}
}

void ABossEnemy::DestroyProjectileTarget()
{
	if (ProjTarg)
	{
		ProjTarg->Destroy();
		ProjTarg = nullptr;
	}
}

void ABossEnemy::SpawnEnemies()
{
	isOKToSpawn = false;
	FVector location = GetActorLocation();
	FRotator rotator = GetActorRotation();
	location.Z = spawnHeight;
	location.X += spawnRadius;
	if (CharacterClass)
	{
		if (!Right || !Right->IsValidLowLevel())
		{
			Right = SpawnEnemyAtLocation(location);
		}
		location.X -= 2 * spawnRadius;
		if (!Left || !Left->IsValidLowLevel())
		{
			Left = SpawnEnemyAtLocation(location);
		}
		location.X += spawnRadius;

		location.Y += spawnRadius;
		if (!Top || !Top->IsValidLowLevel())
		{
			Top = SpawnEnemyAtLocation(location);
		}
		
		location.Y -= 2 * spawnRadius;
		if (!Bottom || !Bottom->IsValidLowLevel())
		{
			Bottom = SpawnEnemyAtLocation(location);
		}
	}
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABossEnemy::nowOKToSpawn,
		30.0, false);
}

ABaseEnemy* ABossEnemy::SpawnEnemyAtLocation(FVector location)
{
 	FRotator rotator = GetActorRotation();
	ABaseEnemy* newEnemy = GetWorld()->SpawnActor<ABaseEnemy>(CharacterClass, location, rotator);
	if (newEnemy)
	{
		ABossHelperController* newEnemyController = GetWorld()->SpawnActor<ABossHelperController>(ABossHelperController::StaticClass(), location, rotator);
		newEnemyController->AssignLocationValues(GetActorLocation(), location);
		newEnemyController->Possess(newEnemy);
	}
	return newEnemy;
}

float ABossEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent,
		EventInstigator, DamageCauser);
	if ((ActualDamage > 0.0f))
	{
		SpawnEnemies();
	}
	return ActualDamage;
}

void ABossEnemy::nowOKToSpawn()
{
	isOKToSpawn = true;
}