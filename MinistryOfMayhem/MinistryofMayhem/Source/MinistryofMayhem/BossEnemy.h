// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseEnemy.h"
#include "BossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MINISTRYOFMAYHEM_API ABossEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	ABossEnemy();
	void BeginPlay() override;
	void ThrowProjectile();
	void StartTrackingPlayer();
	void DestroyProjectileTarget();
	void SpawnEnemies();
	ABaseEnemy* SpawnEnemyAtLocation(FVector location);
	float TakeDamage(float Damage, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	void nowOKToSpawn();
	void Tick(float DeltaTime);
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn> ProjectileTargetClass;
	UPROPERTY(EditAnywhere)
		float ProjectileTargetHeight;
	FTimerHandle TrackTimer;
	FTimerHandle AttackTimer;
	FTimerHandle SpawnTimer;
	class AProjectileTarget* ProjTarg;
	UPROPERTY(EditDefaultsOnly) UAnimationAsset* JumpAnim;
	float spawnRadius = 200.0;
	float spawnHeight = 18;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseEnemy> CharacterClass;
	bool isOKToSpawn = true;
	ABaseEnemy* Top = nullptr;
	ABaseEnemy* Bottom = nullptr;
	ABaseEnemy* Left = nullptr;
	ABaseEnemy* Right = nullptr;
};
