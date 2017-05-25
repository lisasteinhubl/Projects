// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "StairsController.h"


// Sets default values for this component's properties
UStairsController::UStairsController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStairsController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStairsController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStairsController::BringStairsDown(int n)
{
    //Don't forget to connect StairsController in EnemyController and call this command.
    FVector OriginalLocation = StairsActor->GetActorLocation();
    OriginalLocation.Z-= n;
    
    StairsActor->SetActorLocation(OriginalLocation);
    //StairsActor->K2_TeleportTo(, FRotator());
   // Stairs->K2_TeleportTo(FVector(), FRotator());
//   StairsActor->TranslateZ(n);
    
}
