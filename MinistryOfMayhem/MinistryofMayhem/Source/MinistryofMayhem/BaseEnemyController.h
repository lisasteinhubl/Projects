// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseEnemyController.generated.h"


/**
 * 
 */
UCLASS()
class MINISTRYOFMAYHEM_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
public:
		void BeginPlay() override;
		virtual void Tick(float time) override;
		void AttackPlayer();
        UFUNCTION(BlueprintCallable, Category = "UMG Game")
        void SetCurrState();
protected:
	enum State
	{
		Start, Wander, Chase, Attack, Run, PreHeal, Heal, PostHeal, Idle, TrackingPlayer
	};
    
    State curState;
	class APillars* Pillars;
	ANavMeshBoundsVolume* NavMesh;
	FTimerHandle HealTimer;
	FTimerHandle PostHealTimer;
};
