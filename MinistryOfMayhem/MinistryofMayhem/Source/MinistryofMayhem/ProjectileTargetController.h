// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "ProjectileTargetController.generated.h"

/**
 * 
 */
UCLASS()
class MINISTRYOFMAYHEM_API AProjectileTargetController : public AAIController
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void FollowPlayer();
	void HoldPosition();
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
	enum TargetState
	{
		Chase, Stay, BraceForImpact
	};
	TargetState curState;
	float criticalDistance = 100.0f;
};
