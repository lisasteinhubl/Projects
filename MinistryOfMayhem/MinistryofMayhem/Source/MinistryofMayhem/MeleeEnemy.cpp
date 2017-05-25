// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "MeleeEnemy.h"
#include "MinistryofMayhemCharacter.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "MeleeEnemyController.h"
#include "BossHelperController.h"


AMeleeEnemy::AMeleeEnemy()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	HealRate = 2.0f;
	CriticalEnemyDistance = 350.0f;
	AttackRange = 150.0f;
	Damage = 10.0f;
	AIControllerClass = AMeleeEnemyController::StaticClass();

	if (AttackAnim)
	{
		AttackTime = UAnimInstance::GetAnimAssetPlayerLength(AttackAnim);
	}
}

void AMeleeEnemy::StartAttack()
{
	if (AttackAnim)
	{
		GetMesh()->PlayAnimation(AttackAnim, false);
		float attackTime = UAnimInstance::GetAnimAssetPlayerLength(AttackAnim);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AMeleeEnemy::Attack,
			attackTime, true);
	}
}

void AMeleeEnemy::Attack()
{	
	if (AttackAnim)
	{
		GetMesh()->PlayAnimation(AttackAnim, false);
		float attackTime = UAnimInstance::GetAnimAssetPlayerLength(AttackAnim);
	}
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if (player0 && (GetDistanceTo(player0) <= AttackRange))
	{
		AMinistryofMayhemCharacter* Player = Cast<AMinistryofMayhemCharacter>(player0);
		if (Player)
		{
			Player->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	else
	{
		StopAttack();
	}
}

void AMeleeEnemy::StopAttack()
{
	if (AttackAnim)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
}

void AMeleeEnemy::ResetAttackTimer()
{
	//GetWorldTimerManager().ClearTimer(AttackTimer);
	if (GetWorldTimerManager().IsTimerActive(AttackTimer))
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AMeleeEnemy::Attack, AttackTime, true);
	}
}

void AMeleeEnemy::Tick(float DeltaTime)
{
	FRotator curRot = GetActorRotation();
	curRot.Roll = 0.0f;
	SetActorRotation(curRot);
}

float AMeleeEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent,
		EventInstigator, DamageCauser);
	if ((ActualDamage > 0.0f) && (Health > 0.0f))
	{
		if (GetController() && GetController()->IsA<AMeleeEnemyController>()) {
			AMeleeEnemyController* enemyController = static_cast<AMeleeEnemyController*>(GetController());

			if (enemyController)
			{
                enemyController->CheckIfRun();
			}
		}

	}
    else if ((ActualDamage > 0.0f) && (Health <= 0.0f))
    {
        APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
        AMinistryofMayhemCharacter* Player = Cast<AMinistryofMayhemCharacter>(player0);
        if (Player)
        {
        Player->DeadEnemyCounter++;
        Player->AreAllEnemiesDead();
        }
    }
	return ActualDamage;
}
