// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BossHelperController.generated.h"

/**
 * 
 */
UCLASS()
class MINISTRYOFMAYHEM_API ABossHelperController : public AAIController
{
	GENERATED_BODY()
public:
	void AssignLocationValues(FVector protectedLocation, FVector originalLocation);
	void Tick(float DeltaTime) override;
	void ChasePlayer();
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);


private:
	FVector protectedLocation;
	FVector originalLocation;
	float chaseRadius = 1000.0f;

	enum HelperState
	{
		Chase, Idle, ResumePosition, Attack
	};
	
	HelperState curState = Idle;
};
