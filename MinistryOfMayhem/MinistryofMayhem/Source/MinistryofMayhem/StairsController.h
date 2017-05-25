// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "StairsController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINISTRYOFMAYHEM_API UStairsController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStairsController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void BringStairsDown(int n);
    //void InitializeComponent();
	
    UPROPERTY(EditAnywhere, Category = Stairs)
    AActor* StairsActor;
    
};
