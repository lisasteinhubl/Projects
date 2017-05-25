// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Components/TextBlock.h"

#include "MinistryofMayhemPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINISTRYOFMAYHEM_API AMinistryofMayhemPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> MainMenu;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> GameMenu;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> Instructions;
    
    // Variable to hold the widgets after creating
    UUserWidget* mMainMenu;
    UUserWidget* mGameMenu;
    UUserWidget* mInstructions;
    
    // Override BeginPlay()
    virtual void BeginPlay() override;

    
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void GoToInstructions();
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void GoToStartGame();
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void GoToMainMenu();
    
    
    UPROPERTY()
    UTextBlock* HealthTextControl;
    
    UPROPERTY()
    UTextBlock* AmmoTextControl;
    
    UPROPERTY()
    UTextBlock* PointsTextControl;


    
	
	
};
