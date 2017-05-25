
	// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "LongDistanceEnemy.h"
#include "LongDistanceEnemyController.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "MinistryofMayhemCharacter.h"


	ALongDistanceEnemy::ALongDistanceEnemy()
{
    Health = 100.0f;
    MaxHealth = 100.0f;
    AttackRange = 500.0f;
    Damage = 5.0f;
    AIControllerClass = ALongDistanceEnemyController::StaticClass();
}

void ALongDistanceEnemy::BeginPlay()
{
	Super::BeginPlay();
	StartIdleAnimation();
	activeState = Start;
}


void ALongDistanceEnemy::Tick(float DeltaTime)
{    
    APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
    
    if (player0)
    {
        FVector myLoc = GetActorLocation();
        FVector player0Loc = player0->GetActorLocation();
    
        FVector fromEnemyToPlayer = player0->GetActorLocation() - GetActorLocation();
        SetActorRotation(fromEnemyToPlayer.ToOrientationRotator());
        switch(activeState)
        {
            case Start:
            
            
                if( fromEnemyToPlayer.Size() <= AttackRange)
                {
                    activeState = Attack;
                    StartShootAttack();
                }

            
                break;
            case Attack:
                if( fromEnemyToPlayer.Size() > AttackRange)
                {
                    StopAttack();
                    activeState = Start;
                }
                break;
            default:
                break;
            
        }
    }
    
}

void ALongDistanceEnemy::StartShootAttack()
{
    if (AttackAnim)
    {
        GetMesh()->PlayAnimation(AttackAnim, false);
        float attackTime = UAnimInstance::GetAnimAssetPlayerLength(AttackAnim);
        GetWorldTimerManager().SetTimer(AttackTimer, this, &ALongDistanceEnemy::ShootAttackPlayer, 1.0, true); //attackTime = 1.0
    }

}


void ALongDistanceEnemy::ShootAttackPlayer()
{
    APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
    
    if (AttackAnim)
    {
        GetMesh()->PlayAnimation(AttackAnim, false);
        float attackTime = UAnimInstance::GetAnimAssetPlayerLength(AttackAnim);
    }
    player0 = UGameplayStatics::GetPlayerPawn(this, 0);
    if (player0 )//w&& (GetDistanceTo(player0) <= AttackRange))
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

void ALongDistanceEnemy::StopAttack()
{
	if (AttackAnim)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
}

float ALongDistanceEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent,
                                           EventInstigator, DamageCauser);
    if ((ActualDamage > 0.0f) && (Health <= 0.0f))
    {
            APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
            AMinistryofMayhemCharacter* Player = Cast<AMinistryofMayhemCharacter>(player0);
            Player->DeadLongDistanceEnemyCounter++;
            Player->AreAllLongDistanceEnemiesDead();
    }
    return ActualDamage;

}