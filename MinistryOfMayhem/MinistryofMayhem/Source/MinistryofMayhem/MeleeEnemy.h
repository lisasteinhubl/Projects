// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseEnemy.h"
#include "MeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MINISTRYOFMAYHEM_API AMeleeEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Base Enemy")
	float RetrieveHealth() { return GetHealth(); }
	AMeleeEnemy();
	virtual void Attack() override;
	virtual void StartAttack() override;
	virtual void StopAttack() override;
	virtual void ResetAttackTimer() override;
	virtual void Tick(float DeltaTime) override;
	float TakeDamage(float Damage, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
};
