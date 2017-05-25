#pragma once

#include "BaseEnemy.h"
#include "LongDistanceEnemy.generated.h"

/**
*
*/
UCLASS()
class MINISTRYOFMAYHEM_API ALongDistanceEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ALongDistanceEnemy();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void ShootAttackPlayer();

	virtual void StopAttack() override;
	float TakeDamage(float Damage, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	void StartShootAttack();

	enum State { Start, Attack };
	State activeState;
};