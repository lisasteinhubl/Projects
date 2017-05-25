// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MinistryofMayhemGameMode.generated.h"

UCLASS(minimalapi)
class AMinistryofMayhemGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMinistryofMayhemGameMode();
    
protected:
    
    virtual void BeginPlay() override;

};



