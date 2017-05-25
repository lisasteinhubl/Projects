// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "BaseEnemy.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "BaseEnemyController.h"
#include "MinistryofMayhemCharacter.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
    
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GetMesh()->Rotation
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::Heal(float deltaTime)
{
	Health += deltaTime * HealRate;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

float ABaseEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent,
		EventInstigator, DamageCauser);
	if (ActualDamage > 0.0f)
	{
		Health -= ActualDamage;
		if (Health <= 0.0f)
		{
	
			bCanBeDamaged = false;
			float duration = 0.0f; //= PlayAnimMontage(DeathAnim);
			Dead = true;
			AController* controller = GetController();
			if (controller)
			{
                APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
                AMinistryofMayhemCharacter* Player = Cast<AMinistryofMayhemCharacter>(player0);
                
                Player->AddPoints();
                
				controller->UnPossess();
				if (DeathAnim)
				{
					StopAttack();
					GetMesh()->PlayAnimation(DeathAnim, false);
					float deathTime = UAnimInstance::GetAnimAssetPlayerLength(DeathAnim) + 1.0;
					GetWorldTimerManager().SetTimer(DeathTimer, this, &ABaseEnemy::FinalDestruction,
						deathTime, false);
				}
				else
                {
					Destroy();
				}
			}
		}
		else 
		{
			if (HitAnim)
			{
				GetMesh()->PlayAnimation(HitAnim, false);
			}
		}
	}
	return ActualDamage;
}

bool ABaseEnemy::IsDead()
{
	return Dead;
}

void ABaseEnemy::OnStartHeal()
{
	if (HealFX)
	{
		HealPSC = PlayHealEffect(HealFX);
	}
}

void ABaseEnemy::OnStopHeal()
{
	if (HealPSC)
	{
		HealPSC->DeactivateSystem();
	}
}

UParticleSystemComponent* ABaseEnemy::PlayHealEffect(UParticleSystem* ParticleSystem)
{
	
	UParticleSystemComponent* PSC = NULL;
	if (ParticleSystem)
	{
		PSC = UGameplayStatics::SpawnEmitterAttached(ParticleSystem, GetMesh(), TEXT("healingSparksSocket"));
	}
	return PSC;
}

void ABaseEnemy::FinalDestruction()
{
	Destroy();
}

void ABaseEnemy::ResetAttackTimer()
{
	
}

void ABaseEnemy::StartWalkAnimation()
{
	if (WalkAnim)
	{
		
		GetMesh()->PlayAnimation(WalkAnim, true);
	}
}

void ABaseEnemy::StartIdleAnimation()
{
	if (StandAnim)
	{
		GetMesh()->PlayAnimation(StandAnim, false);
	}
}

float ABaseEnemy::StartHealAnimation()
{
	if (CrouchAnim)
	{
		GetMesh()->PlayAnimation(CrouchAnim, false);
		return UAnimInstance::GetAnimAssetPlayerLength(CrouchAnim);
	}
	return 0.0f;
}

float ABaseEnemy::StopHealAnimation()
{
	if (StandAnim)
	{

		GetMesh()->PlayAnimation(StandAnim, false);
		return UAnimInstance::GetAnimAssetPlayerLength(StandAnim);
	}
	return 0.0f;
}



