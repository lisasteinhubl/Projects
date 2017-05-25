// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class MINISTRYOFMAYHEM_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly) UAnimationAsset* AttackAnim;
	UPROPERTY(EditDefaultsOnly) UAnimationAsset* DeathAnim;
	UPROPERTY(EditDefaultsOnly) UAnimationAsset* HitAnim;
	UPROPERTY(EditDefaultsOnly) UAnimationAsset* WalkAnim;
	UPROPERTY(EditDefaultsOnly) UAnimationAsset* CrouchAnim;
	UPROPERTY(EditDefaultsOnly) UAnimationAsset* StandAnim;
	FTimerHandle DeathTimer;
	FTimerHandle AttackTimer;
	FTimerHandle HitTimer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Damage)
		float Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Damage)
		float Damage;
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* HealFX;
	UPROPERTY(Transient)
		class UParticleSystemComponent* HealPSC;
	float MaxHealth = 100.0f;
	float HealRate = 0.5f;
	float CriticalEnemyDistance = 350.0f;
	float AttackRange = 150.0f;
	float MaxFollowDistance = 700.0f;
	float AttackTime = 0.0f;
	float WalkSpeed = 100.0f;
	float JogSpeed = 400.0f;
	
	bool Dead = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack() {}
	virtual void StartAttack() {}
	virtual void StopAttack() {}

	virtual void StartWalkAnimation();
	virtual float StartHealAnimation();
	virtual float StopHealAnimation();
	virtual void StartIdleAnimation();

	UFUNCTION(BlueprintCallable, Category = "Base Enemy")
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Base Enemy")
	float GetMaxHealth() { return MaxHealth; }

	float GetHealRate() { return HealRate; }
	float GetCriticalEnemyDistance() { return CriticalEnemyDistance; }
	float GetAttackRange() { return AttackRange; }
	float GetMaxFollowDistance() { return MaxFollowDistance; }
	float GetWalkSpeed() { return WalkSpeed; }
	float GetJogSpeed() { return JogSpeed; }
	void Heal(float deltaTime);
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	bool IsDead();
	void OnStartHeal();
	void OnStopHeal();
	void FinalDestruction();
	virtual void ResetAttackTimer();
	UParticleSystemComponent* PlayHealEffect(UParticleSystem* ParticleSystem);
    
    
 
    
};
