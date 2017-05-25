// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "BossHelperController.h"
#include "BaseEnemy.h"


void ABossHelperController::AssignLocationValues(FVector protectedLocation, FVector originalLocation)
{
	this->protectedLocation = protectedLocation;
	this->originalLocation = originalLocation;
}

void ABossHelperController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABaseEnemy* myPawn = static_cast<ABaseEnemy*>(GetPawn());
	if (!myPawn)
	{
		return;
	}
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!player0)
	{
		return;
	}

	if ((curState == Idle || curState == ResumePosition) && (FVector::Dist(player0->GetActorLocation(), protectedLocation) < chaseRadius))
	{
		StopMovement();
		ChasePlayer();
		curState = Chase;
	}
	else if (curState == Chase && (FVector::Dist(player0->GetActorLocation(), protectedLocation) > chaseRadius))
	{
		StopMovement();
		MoveToLocation(originalLocation);
		curState = ResumePosition;
	}
	else if (curState == Attack)
	{
		if (myPawn && player0)
		{
			FVector myLoc = myPawn->GetActorLocation();
			FVector player0Loc = player0->GetActorLocation();

			FVector fromEnemyToPlayer = player0->GetActorLocation() - myPawn->GetActorLocation();
			myPawn->SetActorRotation(fromEnemyToPlayer.ToOrientationRotator());

			if (FVector::Dist(myLoc, player0Loc) > myPawn->GetAttackRange())
			{
				ABaseEnemy* enemy = static_cast<ABaseEnemy*>(GetPawn());
				enemy->StopAttack();
				ChasePlayer();
				curState = Chase;
			}

		}
	}

}

void ABossHelperController::ChasePlayer()
{
	ABaseEnemy* myEnemy = static_cast<ABaseEnemy*>(GetPawn());
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if (myEnemy && myEnemy->GetCharacterMovement())
	{
		myEnemy->GetCharacterMovement()->MaxWalkSpeed = myEnemy->GetJogSpeed();
		myEnemy->StartWalkAnimation();
	}
	if (player0)
	{
		FVector enemyLocation = GetPawn()->GetActorLocation();
		FVector playerLocation = player0->GetActorLocation();
		//if (enemyLocation)
		MoveToActor(player0);
	}
}



void ABossHelperController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{

	ABaseEnemy* enemy = static_cast<ABaseEnemy*>(GetPawn());
	if (!enemy)
	{
		return;
	}

	if (curState == Chase && (Result == EPathFollowingResult::Success))
	{
		curState = Attack;
		enemy->StartAttack();
	}
	else if ((curState == ResumePosition) && (Result == EPathFollowingResult::Success)) {
		enemy->StartIdleAnimation();
		curState = Idle;
	}
}