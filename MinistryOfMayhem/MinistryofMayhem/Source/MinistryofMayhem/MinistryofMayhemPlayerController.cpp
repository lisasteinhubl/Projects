// Fill out your copyright notice in the Description page of Project Settings.

#include "MinistryofMayhem.h"
#include "Blueprint/UserWidget.h"
#include "BaseEnemyController.h"
#include "BaseEnemy.h"
#include "Components/TextBlock.h"
#include "MinistryofMayhemPlayerController.h"
#include "Engine.h"




void AMinistryofMayhemPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Check if the Main menu asset is assigned in the blueprint
    if (MainMenu)
    {
        // Create the main menu widget
        mMainMenu = CreateWidget<UUserWidget>(this, MainMenu);
        
        if (mMainMenu) {
            //let add it to the view port
            mMainMenu->AddToViewport();
        }
        
        //Show the cursor so user can select a button
        bShowMouseCursor = false;
        
    }
    
    //Check if the Game Menu asset is assigned in the blueprint
    if (GameMenu)
    {
        //Create the game menu widget
        mGameMenu = CreateWidget<UUserWidget>(this, GameMenu);
        
        //TextControls are used to edit the text being displayed
        UWidget* HealthWidget = mGameMenu->GetWidgetFromName("HealthText");
        HealthTextControl = (UTextBlock*) HealthWidget;
        
        
        UWidget* AmmoWidget = mGameMenu->GetWidgetFromName("AmmoText");
        AmmoTextControl = (UTextBlock*) AmmoWidget;

        
        
        UWidget* PointsWidget = mGameMenu->GetWidgetFromName("PointsText");
        PointsTextControl = (UTextBlock*) PointsWidget;
        

    }
    
    if (Instructions)
    {
        //Create the instructions widget
        mInstructions = CreateWidget<UUserWidget>(this, Instructions);
        
    }
    
    
    
    SetInputMode(FInputModeGameAndUI());

    
}

void AMinistryofMayhemPlayerController::GoToStartGame()
{
    //First remove the main menu
    if (mMainMenu) {
        mMainMenu->RemoveFromViewport();
        for (TObjectIterator<AActor> Itr; Itr; ++Itr)
        {
            if (Itr->IsA(ABaseEnemy::StaticClass()))
            {
                ABaseEnemy* mEnemy = (ABaseEnemy*) *Itr;
                ABaseEnemyController* enemyController = (ABaseEnemyController*) mEnemy->GetController();
                if (enemyController != nullptr) {
                    enemyController->SetCurrState();

                }
                
            }
        }

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Only melee first level!"));
		}

		
        
    }
    
    //Add game menu to screen
    if (mGameMenu) {
        mGameMenu->AddToViewport();

    }
    
    
}

void AMinistryofMayhemPlayerController::GoToInstructions()
{
    
    if (mMainMenu) {
        mMainMenu->RemoveFromViewport();
    }
    
    //Add instructions blue print
    if (mInstructions) {
        mInstructions->AddToViewport();
    }
    
}

void AMinistryofMayhemPlayerController::GoToMainMenu()
{
    
    if (mInstructions) {
        mInstructions->RemoveFromViewport();
    }
    

    
    //Add main menu again
    mMainMenu->AddToViewport();
}




