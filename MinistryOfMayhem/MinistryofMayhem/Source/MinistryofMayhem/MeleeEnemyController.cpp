// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "MeleeEnemyController.h"
#include "NavMeshContainer.h"
#include "MeleeEnemy.h"
#include "Pillars.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"



void AMeleeEnemyController::BeginPlay()
{
	Super::BeginPlay();
	for (TObjectIterator<ANavMeshContainer> Itr; Itr; ++Itr)
	{

		if (Itr->IsA(ANavMeshContainer::StaticClass()))
		{
			NavMesh = Itr->GetSmallMesh();
		}
	}
}

void AMeleeEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABaseEnemy* myPawn = static_cast<ABaseEnemy*>(GetPawn());
	if (!myPawn)
	{
		return;
	}
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	if (player0 && player0->bCanBeDamaged)
	{
		if (GetPawn())
		{
			float distanceToPlayer = GetPawn()->GetDistanceTo(player0);
		}
	}
	else
	{
		return;
	}
	if (curState == Start)
	{
		WanderAround();
		curState = Wander;
	}

	else if (curState == Idle) {
		myPawn->StartIdleAnimation();
		return;
	}


	else if (curState == Heal)
	{
		myPawn->Heal(DeltaTime);
		if (myPawn->GetHealth() == myPawn->GetMaxHealth())
		{
			myPawn->OnStopHeal();
			WanderAround();
			curState = Wander;
		}
	}
	else if ((curState == Wander) && player0 && (GetPawn()->GetDistanceTo(player0) < myPawn->GetCriticalEnemyDistance()))
	{
		ChasePlayer();
		curState = Chase;
	}
	else if (curState == Chase && player0 && (GetPawn()->GetDistanceTo(player0) > myPawn->GetMaxFollowDistance()))
	{
		StopMovement();
		myPawn->StopAttack();
		WanderAround();
		curState = Wander;
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

void AMeleeEnemyController::WanderAround()
{
	ABaseEnemy* myEnemy = static_cast<ABaseEnemy*>(GetPawn());
	if (myEnemy && myEnemy->GetCharacterMovement())
	{
		myEnemy->GetCharacterMovement()->MaxWalkSpeed = myEnemy->GetWalkSpeed();
		myEnemy->StartWalkAnimation();
	}

	if (NavMesh)
	{
		FVector currentLocation = GetPawn()->GetActorLocation();
		FVector location;
		FVector bounds;

		NavMesh->GetActorBounds(false, location, bounds);
		int newX = rand() % (int)bounds.X;
		int newY = rand() % (int)bounds.Y;
		location.X += newX;
		location.Y += newY;
		location.Z = currentLocation.Z;
		MoveToLocation(location);
	}
}

void AMeleeEnemyController::ChasePlayer()
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

void AMeleeEnemyController::RunAway()
{
	APawn* player0 = UGameplayStatics::GetPlayerPawn(this, 0);
	ABaseEnemy* myEnemy = static_cast<ABaseEnemy*>(GetPawn());
	if (myEnemy && myEnemy->GetCharacterMovement())
	{
		myEnemy->StopAttack();
		myEnemy->StartWalkAnimation();
		myEnemy->GetCharacterMovement()->MaxWalkSpeed = myEnemy->GetJogSpeed();
	}

	if (Pillars != nullptr)
	{
		int numPillars = Pillars->getPillarArray().Num();
		int index = rand() % numPillars;

		AActor* moveActor = Pillars->getPillarArray()[index];
		FVector columnLocation = moveActor->GetActorLocation();
		FVector currentLocation = GetPawn()->GetActorLocation();
		FVector playerLocation = player0->GetActorLocation();
		FVector origin;
		FVector columnBounds;
		moveActor->GetActorBounds(true, origin, columnBounds);

		float columnRadius = (float)sqrt(pow(columnBounds.X, 2) + pow(columnBounds.Y, 2));

		FVector columnToPlayer = playerLocation - columnLocation;
		columnToPlayer.Normalize();
		columnToPlayer *= -1;
		columnLocation.Z = currentLocation.Z;
		FVector newLocation = columnLocation + columnToPlayer * columnRadius;
		MoveToLocation(newLocation);
	}
	curState = Run;
}

void AMeleeEnemyController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{

	ABaseEnemy* enemy = static_cast<ABaseEnemy*>(GetPawn());
	if (!enemy)
	{
		return;
	}

	if ((enemy) && curState == Chase)
	{
		curState = Attack;
		enemy->StartAttack();
	}
	else if ((enemy) && (curState == Run)) {

		float timeToHeal = enemy->StartHealAnimation();
		GetWorldTimerManager().SetTimer(HealTimer, this, &AMeleeEnemyController::StartHeal,
			timeToHeal, false);
		curState = PreHeal;
	}
	else if (curState == Wander)
	{
		WanderAround();
	}
}

void AMeleeEnemyController::StartHeal()
{
	curState = Heal;
	ABaseEnemy* myPawn = static_cast<ABaseEnemy*>(GetPawn());
	if (myPawn)
	{
		myPawn->OnStartHeal();
	}
}

void AMeleeEnemyController::CheckIfRun()
{
	ABaseEnemy* myPawn = static_cast<ABaseEnemy*>(GetPawn());
	if (!myPawn)
	{
		return;
	}

	if (curState == Run)
	{
		return;
	}

	if (curState == Heal)
	{
		myPawn->OnStopHeal();
	}

	if (myPawn->GetHealth() < (myPawn->GetMaxHealth() * 0.35) && (curState != Run))
	{
		if (curState == Attack)
		{
			ABaseEnemy* enemy = static_cast<ABaseEnemy*>(GetPawn());
			enemy->StopAttack();
		}
		RunAway();
	}
	else if (curState != Attack)
	{
		StopMovement();
		ChasePlayer();
		curState = Chase;
	}
}