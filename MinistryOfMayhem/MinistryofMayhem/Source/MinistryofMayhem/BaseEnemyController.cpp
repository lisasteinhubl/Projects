// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "BaseEnemyController.h"
#include "Blueprint/UserWidget.h"
#include <stdlib.h>  
#include <algorithm> 
#include "BaseEnemy.h"
#include "NavMeshContainer.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"
#include "Pillars.h"

void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();
	curState = Idle;
	Pillars = nullptr;
	
	for (TObjectIterator<APillars> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(APillars::StaticClass()))
		{
			Pillars = *Itr;
		}
	}

}

void ABaseEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




void ABaseEnemyController::AttackPlayer()
{
	curState = Attack;
}




void ABaseEnemyController::SetCurrState()
{
    this->curState = Start;
    
}





