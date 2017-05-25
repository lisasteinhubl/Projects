// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MinistryofMayhem.h"
#include "MinistryofMayhemGameMode.h"
#include "MinistryofMayhemCharacter.h"

AMinistryofMayhemGameMode::AMinistryofMayhemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMinistryofMayhemGameMode::BeginPlay()
{
    Super::BeginPlay();

}


