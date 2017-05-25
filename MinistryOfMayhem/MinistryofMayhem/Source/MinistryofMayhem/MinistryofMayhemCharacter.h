    // Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MinistryofMayhemCharacter.generated.h"

UCLASS(config=Game)
class AMinistryofMayhemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMinistryofMayhemCharacter();
    void BeginPlay() override;
    void MeleeAttack();
    void StartMeleeAttack();
    void StopMeleeAttack();
    void DeathSequence();
    void RestartGame();
    bool isDead;
    FTimerHandle timeHand;
    FTimerHandle pointTimerHandle;
    
    float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    void UpdatePoints();
    void AddPoints();
    
    void TimerOver();
    int pointsPerHit = 10;
    
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

    UPROPERTY(EditDefaultsOnly, Category = Death)
    class UAnimMontage* DeathAnim;
    
    UPROPERTY(EditDefaultsOnly, Category = Melee)
    class UAnimMontage* MeleeAnimation;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
    float PlayerHealth = 1.0f;
        
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    float GetHealth(){return (PlayerHealth/100);};
    

    UPROPERTY(EditAnywhere, Category = Damage)
    float DamageDonePerHit = 50.0f;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Points)
    float PlayerPoints = 0.0f;
    
    //Widgets that appear when the player wins/loses
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> GameOver;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> YouWin;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> MainMenu;
    
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void GoToMainMenu();
    
    UUserWidget* mGameOver;
    UUserWidget* mYouWin;
    UUserWidget* mMainMenu;

	bool MeleeAnimationComplete = true;
    
    //Properties for player sound
    UPROPERTY(EditDefaultsOnly, Category = Sound)
    class USoundCue* FootstepSound;
    
    UPROPERTY(Transient)
    class UAudioComponent* FootstepAC;
    
    UPROPERTY(EditDefaultsOnly, Category = Sound)
    class USoundCue* BackgroundLoopSound;
    
    UPROPERTY(Transient)
    class UAudioComponent* BackgroundLoopAC;
    
    UAudioComponent* PlaySound(USoundCue* Sound);
    
	bool canFire = false;
    
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
    
	FTimerHandle deathTimer;
    FTimerHandle startTimer;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AMeleeWeapon> MeleeWeaponClass;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class ARifleWeapon> RifleWeaponClass;

	bool canSwitch = false;
	bool levelDone = false;
	bool isRifle = false;
	void OnStartFire();
	void OnStopFire();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    int DeadEnemyCounter = 0;
    void AreAllEnemiesDead();
    int DeadLongDistanceEnemyCounter;
    void AreAllLongDistanceEnemiesDead();

    
    UPROPERTY(EditAnywhere, Category = Stairs)
    AActor* StairsActor;

    UPROPERTY(EditAnywhere, Category = Walls)
    AActor* Wall1;
    
    UPROPERTY(EditAnywhere, Category = Walls)
    AActor* Wall2;
    
    UPROPERTY(EditAnywhere, Category = Walls)
    AActor* Wall3;
    
    UPROPERTY(EditAnywhere, Category = Walls)
    AActor* Wall4;
    
    UPROPERTY(EditAnywhere, Category = Walls)
    AActor* BossLocationCube;
    
    
    void Tick(float DeltaSeconds) override;
    
    bool wallsMoving;
    FVector wall1_original_loc;
    FVector wall2_original_loc;
    FVector wall3_original_loc;
    FVector wall4_original_loc;
    
    UPROPERTY(EditAnywhere, Category = Boss)
    class ATargetPoint* bossTarget;
    
    
    UPROPERTY(EditAnywhere, Category = Boss)
    TSubclassOf<APawn> PawnClass;

private:

	class ARifleWeapon* MyRifleWeapon;
	class AMeleeWeapon* MyMeleeWeapon;

    
};

