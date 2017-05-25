// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "ProjectileTargetController.h"

void AProjectileTargetController::BeginPlay()
{
	Super::BeginPlay();
	curState = Stay;
}

void AProjectileTargetController::Tick(float DeltaTime)
{
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if ((curState == Stay) && player0 && (GetPawn()->GetDistanceTo(player0) > criticalDistance))
	{
		curState = Chase;
		FollowPlayer();
	}
}

void AProjectileTargetController::HoldPosition()
{
	StopMovement();
	curState = BraceForImpact;
}

void AProjectileTargetController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if ((curState == Chase) && (Result == EPathFollowingResult::Success))
	{
		curState = Stay;
	}
}

void AProjectileTargetController::FollowPlayer()
{
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if (player0)
	{
		MoveToActor(player0, 0.0f);
	}
}


