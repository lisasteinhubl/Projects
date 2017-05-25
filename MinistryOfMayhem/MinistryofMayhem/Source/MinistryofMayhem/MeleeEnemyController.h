// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseEnemyController.h"
#include "MeleeEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class MINISTRYOFMAYHEM_API AMeleeEnemyController : public ABaseEnemyController
{
	GENERATED_BODY()
public:
		void BeginPlay() override;
		void Tick(float time) override;
		void RunAway();
		void WanderAround();
		void ChasePlayer();
		void StartHeal();
		void CheckIfRun();
		void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

};
