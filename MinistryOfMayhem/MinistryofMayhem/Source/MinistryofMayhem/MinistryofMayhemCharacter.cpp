// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MinistryofMayhem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "MinistryofMayhemCharacter.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "MeleeEnemy.h"
#include "StairsController.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "MeleeWeapon.h"
#include "RifleWeapon.h"
#include "Engine/TargetPoint.h"
#include "BossEnemy.h"


//////////////////////////////////////////////////////////////////////////
// AMinistryofMayhemCharacter

AMinistryofMayhemCharacter::AMinistryofMayhemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MyMeleeWeapon = nullptr;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    
    //CHANGING THIS -- JumpZVelocity was 600.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
    
    //CHANGING THIS -- AirControl was 0.2f
	GetCharacterMovement()->AirControl = 0.6f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
    //CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
    isDead = false;
    
    DeadLongDistanceEnemyCounter = 0;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMinistryofMayhemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings

    PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &AMinistryofMayhemCharacter::StartMeleeAttack);
    PlayerInputComponent->BindAction("MeleeAttack", IE_Released, this, &AMinistryofMayhemCharacter::StopMeleeAttack);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMinistryofMayhemCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMinistryofMayhemCharacter::OnStopFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMinistryofMayhemCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMinistryofMayhemCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMinistryofMayhemCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMinistryofMayhemCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMinistryofMayhemCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMinistryofMayhemCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMinistryofMayhemCharacter::OnResetVR);
}


void AMinistryofMayhemCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMinistryofMayhemCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMinistryofMayhemCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMinistryofMayhemCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMinistryofMayhemCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMinistryofMayhemCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
        
        //Add sound
        //FootstepAC = PlaySound(FootstepSound);
        //FootstepAC->Play();
        
        
	}
}

void AMinistryofMayhemCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void AMinistryofMayhemCharacter::BeginPlay()
{
	Super::BeginPlay();
    
    
    
    
    //	MyMeleeWeapon = nullptr;
	// Spawn the weapon, if one was specified
	if (RifleWeaponClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Need to set rotation like this because otherwise gun points down
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			/*FRotator Rotation(0.0f, 90.0f, 0.0f);*/
			// Spawn the Weapon
			MyRifleWeapon = World->SpawnActor<ARifleWeapon>(RifleWeaponClass, FVector::ZeroVector,
				Rotation, SpawnParams);
			MyRifleWeapon->SetMyOwner(this);
			if (MyRifleWeapon)
			{
				// This is attached to "WeaponPoint" which is defined in the skeleton
				MyRifleWeapon->AttachToComponent(GetMesh(),
					FAttachmentTransformRules::KeepRelativeTransform,
					TEXT("RiflePoint"));
			}
		}
	}

	if (MeleeWeaponClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Need to set rotation like this because otherwise gun points down
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			//FRotator Rotation(-90.0f, 0.0f, -90.0f);
			// Spawn the Weapon
			MyMeleeWeapon = World->SpawnActor<AMeleeWeapon>(MeleeWeaponClass, FVector::ZeroVector,
				Rotation, SpawnParams);
			//MyMeleeWeapon->SetMyOwner(this);
			if (MyMeleeWeapon)
			{
				// This is attached to "WeaponPoint" which is defined in the skeleton
				MyMeleeWeapon->AttachToComponent(GetMesh(),
					FAttachmentTransformRules::KeepRelativeTransform,
					TEXT("MeleeHolster"));
			}
		}
	}

    
    if (GameOver) {
        mGameOver = CreateWidget<UUserWidget>(GetWorld()->GetGameInstance(), GameOver);
        
    }
    if (YouWin) {
        mYouWin = CreateWidget<UUserWidget>(GetWorld()->GetGameInstance(), YouWin);
    }
    
    if (MainMenu) {
        mMainMenu = CreateWidget<UUserWidget>(GetWorld()->GetGameInstance(), MainMenu);

    }
    
    //Start background loop
    BackgroundLoopAC = PlaySound(BackgroundLoopSound);
    BackgroundLoopAC->Play();
    
    //Start points timer
    GetWorldTimerManager().SetTimer(pointTimerHandle, this, &AMinistryofMayhemCharacter::UpdatePoints, 10.0f, true);
    
    /*wall1_original_loc = Wall1->GetActorLocation();
    wall2_original_loc = Wall2->GetActorLocation();
    wall3_original_loc = Wall3->GetActorLocation();
    wall4_original_loc = Wall4->GetActorLocation();
    wallsMoving = true;*/

    
}

//Should be called by AttackButtonPressed();
void AMinistryofMayhemCharacter::StartMeleeAttack()
{

	//FRotator Rotation(0.0f, -90.0f, 0.0f);
	//MyRifleWeapon->SetActorRotation(Rotation);
	MyRifleWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("RifleHolster"));

	MyMeleeWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("MeleePoint"));

	canFire = false;

    if(MeleeAnimationComplete)
    {
		if (MeleeAnimation)
		{
			MeleeAnimationComplete = false;
			GetMesh()->PlayAnimation(MeleeAnimation, true);
			float animationDuration = UAnimInstance::GetAnimAssetPlayerLength(MeleeAnimation);

			//float animationDuration = UAnimInstance::GetInstanceAssetPlayerLength(MeleeAnimation);
			GetWorldTimerManager().SetTimer(timeHand, this, &AMinistryofMayhemCharacter::MeleeAttack, animationDuration, true);
		}
        
    }
    //Play melee animation
    //Do damage to enemy
    //
}

void AMinistryofMayhemCharacter::MeleeAttack()
{
    
    static FName HandTag = FName(TEXT("head"));
	

    //Start from the Muzzle position
    FVector StartPos = this->GetActorLocation();
    FVector Forward = this->GetActorForwardVector();
    
    //SELF CALCULATED
    FVector EndPos = StartPos + (Forward*150.0); //150.0 is weapon range.
    
    FCollisionQueryParams TraceParams(HandTag, true, Instigator);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = true;
    
    //This fires the ray and checks against all objects for collision.
    FHitResult Hit(ForceInit);
    GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, EndPos, FCollisionObjectQueryParams::AllObjects, TraceParams);
        ABaseEnemy* mEnemy = Cast<ABaseEnemy>(Hit.GetActor());
    if(mEnemy)
    {
   
        
        mEnemy->TakeDamage(40.0f, FDamageEvent(), GetInstigatorController(), this);
		//if (mEnemy->IsA<ABossEnemy>() && mYouWin)
		//{
		//	mYouWin->AddToViewport();
		//}
        
    }

    
    if(MeleeAnimation)
    {
        GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        GetWorldTimerManager().ClearTimer(timeHand);
		MeleeAnimationComplete = true;
    }

	//FRotator Rotation(0.0f, 0.0f, -90.0f);
	MyRifleWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("RiflePoint"));

	MyMeleeWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("MeleeHolster"));

	canFire = true;

}

void AMinistryofMayhemCharacter::StopMeleeAttack()
{

}

float AMinistryofMayhemCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    if(ActualDamage > 0.0f)
    {
        PlayerHealth-= ActualDamage*0.01;
        if(PlayerHealth <= 0.0f)
        {
            bCanBeDamaged = false;

            GetMesh()->PlayAnimation(DeathAnim, false);
            float animationDuration = UAnimInstance::GetAnimAssetPlayerLength(DeathAnim) - 0.25;
            GetWorldTimerManager().SetTimer(deathTimer, this, &AMinistryofMayhemCharacter::DeathSequence,animationDuration, false);
            
            APlayerController* PC = Cast<APlayerController>(GetController());
            if(PC)
            {
                PC->SetCinematicMode(true, true, true);
            }
            
            GetWorldTimerManager().SetTimer(deathTimer, this, &AMinistryofMayhemCharacter::RestartGame,animationDuration, false);
            
//            StopMeleeAttack(); //Alternatively (Play TakeDamage animation)
            
            //Causes SegFault -- Check AddToViewport code?
            //mGameOver->AddToViewport();

        }
    }
    return ActualDamage;
}

void AMinistryofMayhemCharacter::DeathSequence()
{
    GetMesh()->Deactivate();
    isDead = true;
    Destroy();
}

void AMinistryofMayhemCharacter::RestartGame()
{
    //Restart game
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AMinistryofMayhemCharacter::GoToMainMenu()
{
    mGameOver->RemoveFromViewport();
    mYouWin->RemoveFromViewport();
    mMainMenu->AddToViewport();
}


UAudioComponent* AMinistryofMayhemCharacter::PlaySound(USoundCue* Sound)
{
    UAudioComponent* AC = NULL;
    if (Sound)
    {
        AC = UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
    }
    return AC;
}

void AMinistryofMayhemCharacter::AreAllEnemiesDead()
{
    if(DeadEnemyCounter == 6)
    {
		levelDone = true;
		canSwitch = true;
		canFire = true;
//        UStairsController* stairsController = InitializeComponent();
//        stairsController->BringStairsDown(1430);
        FVector OriginalLocation = StairsActor->GetActorLocation();
        OriginalLocation.Z-= 1700;
//        OriginalLocation.X-= 7730;
        
        StairsActor->SetActorLocation(OriginalLocation);
        
        
        
        
    }
    
//        while(originalLocationWall1.X >= originalLocationWall1.X-5360
//              &&
//              originalLocationWall1.Z <= originalLocationWall1.Z+2550
//              &&
//              originalLocationWall2.X >=originalLocationWall2.X-5360
//              &&
//              originalLocationWall2.Z <= originalLocationWall2.Z+2550
//              &&
//              originalLocationWall3.X >= originalLocationWall3.X-5360
//              &&
//              originalLocationWall3.Z <= originalLocationWall3.Z+2550
//              &&
//              originalLocationWall4.X >= originalLocationWall4.X-5360
//              &&
//              originalLocationWall4.Z <= originalLocationWall4.Z+2550
//              )
//        {
//            Wall1->SetActorLocation(FVector(OriginalLocation.X-100, OriginalLocation.Y, OriginalLocation.Z+100));
//            Wall2->SetActorLocation(FVector(OriginalLocation.X-100, OriginalLocation.Y, OriginalLocation.Z+100));
//            Wall3->SetActorLocation(FVector(OriginalLocation.X-100, OriginalLocation.Y, OriginalLocation.Z+100));
//            Wall4->SetActorLocation(FVector(OriginalLocation.X-100, OriginalLocation.Y, OriginalLocation.Z+100));
//        }
//        
//    }
}

void AMinistryofMayhemCharacter::AreAllLongDistanceEnemiesDead()
{
    if(DeadLongDistanceEnemyCounter == 6)
    {
        //Move player to boss battle.
       // FVector OriginalLocation = EnemyPlatform->GetActorLocation();
       // OriginalLocation.Z-= 1700;
        //        OriginalLocation.X-= 7730;
        
        //EnemyPlatform->SetActorLocation(OriginalLocation);
        
        //this->SetActorLocation(BossLocationCube->GetActorLocation());
        FVector tempLocation = bossTarget->GetActorLocation();
        FRotator tempRotator = bossTarget->GetActorRotation();
		canFire = false;

        if(PawnClass)
        {
            APawn* Char = GetWorld()->SpawnActor<APawn>(PawnClass, tempLocation, tempRotator);
            if (Char)
            {
                // Spawn the AI controller for the character
                Char->SpawnDefaultController();
                
            }
        }
        
    }
}

void AMinistryofMayhemCharacter::AddPoints()
{
    //Add points
    PlayerPoints += pointsPerHit;
}

void AMinistryofMayhemCharacter::UpdatePoints()
{
    if (pointsPerHit == 0) {
        return;
    }
    pointsPerHit -= 1;
}

void AMinistryofMayhemCharacter::Tick(float DeltaTime)
{
//    if(wallsMoving == true)
//    {
//        FVector New_Wall1Loc = Wall1->GetActorLocation();
//        New_Wall1Loc.X =  (wall1_original_loc.X-5360)*DeltaTime;
////        New_Wall1Loc.Z = (wall1_original_loc.Z+2550)*DeltaTime;
//        Wall1->SetActorLocation(New_Wall1Loc);
//        
//        FVector New_Wall2Loc = Wall2->GetActorLocation();
//        New_Wall2Loc.X =  (wall2_original_loc.X-5360)*DeltaTime;
////        New_Wall2Loc.Z = (wall2_original_loc.Z+2550)*DeltaTime;
//        Wall2->SetActorLocation(New_Wall2Loc);
//        
//        FVector New_Wall3Loc = Wall3->GetActorLocation();
//        New_Wall3Loc.X =  (wall3_original_loc.X-5360)*DeltaTime;
////        New_Wall3Loc.Z = (wall3_original_loc.Z+2550)*DeltaTime;
//        Wall3->SetActorLocation(New_Wall3Loc);
//
//        FVector New_Wall4Loc = Wall4->GetActorLocation();
//        New_Wall4Loc.X =  (wall4_original_loc.X-5360)*DeltaTime;
////        New_Wall4Loc.Z = (wall4_original_loc.Z+2550)*DeltaTime;
//        Wall4->SetActorLocation(New_Wall4Loc);
//    }
    
//        Wall2->SetActorLocation(FVector(wall2_original_loc.X-100*DeltaTime, wall2_original_loc.Y, wall2_original_loc.Z+100*DeltaTime));
//        Wall3->SetActorLocation(FVector(wall3_original_loc.X-100*DeltaTime, wall3_original_loc.Y, wall3_original_loc.Z+100*DeltaTime));
//        Wall4->SetActorLocation(FVector(wall4_original_loc.X-100*DeltaTime, wall4_original_loc.Y, wall4_original_loc.Z+100*DeltaTime));
    
    
//    if(Wall1->GetActorLocation().X >= wall1_original_loc.X-5360 && Wall1->GetActorLocation().Z <= wall1_original_loc.Z+2550)
//    {
//        wallsMoving = false;
//    }
    
    /*
    if(PressurePlate)
    {
        if(PressurePlate->IsOverlappingActor(triggerActor))
        {
            FVector Wall1Loc = Wall1->GetActorLocation();
            FVector Wall2Loc = Wall2->GetActorLocation();
            FVector Wall3Loc = Wall3->GetActorLocation();
            FVector Wall4Loc = Wall4->GetActorLocation();
            
            Wall1Loc.Z -= 1000;
            Wall2Loc.Z -= 1000;
            Wall3Loc.Z -= 1000;
            Wall4Loc.Z -= 1000;
            
            Wall1->SetActorLocation(Wall1Loc);
            Wall2->SetActorLocation(Wall2Loc);
            Wall3->SetActorLocation(Wall3Loc);
            Wall4->SetActorLocation(Wall4Loc);
        }
    }
   */
    
    
}

void AMinistryofMayhemCharacter::OnStartFire()
{

	if (MyRifleWeapon != nullptr && canFire && levelDone)
	{
		MyRifleWeapon->OnStartFire();
	}
}

void AMinistryofMayhemCharacter::OnStopFire()
{
	if (MyRifleWeapon != nullptr)
	{
		MyRifleWeapon->OnStopFire();
	}

}



