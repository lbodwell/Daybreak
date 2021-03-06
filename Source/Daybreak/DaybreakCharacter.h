// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DaybreakSword.h"
#include "DaybreakArmor.h"
#include "GameFramework/PlayerController.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "DaybreakCharacter.generated.h"

class ADaybreakSword;

UCLASS()
class ADaybreakCharacter : public ACharacter {
    GENERATED_BODY()

    virtual void BeginPlay() override;

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;
	
	/** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
private:
	APlayerController* playerController;
	UAudioComponent* attackSwingSound;
	UAudioComponent* anvilInteractSound;
	UAudioComponent* playerHurtSound;

public:
    ADaybreakCharacter();

	virtual void Tick(float DeltaTime) override;

    float BaseTurnRate;
    float BaseLookUpRate;

    /** Whether player is currently attacking and should not be able to attack again yet. */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=State)
	bool Attacking;
	
	/** Whether player is currently hitting and enemies should receive damage. */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=State)
	bool Hitting;
	
	/** Directional turning speed. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=State)
    float TurningVelocity;
	
	/** Player base health at full. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
    float BaseHealth;
	
	/** Player health currently. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
    float Health;
	
	/** Player Dark Stone storage. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
    float DarkStone;

	/** Player Day Progress storage. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
	int DayCount = 1;
	
	/** Player sword object for blueprints. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=Equipment)
    class ADaybreakSword* Sword;
	
	/** Player armor object for blueprints. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=Equipment)
    class ADaybreakArmor* Armor;

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(int amount);
	
	/** Player sword object for C++. */
	class ADaybreakSword* GetSword();
	
	/** Player armor object for C++. */
	class ADaybreakArmor* GetArmor();
	
	UInputComponent* GetPlayerInputComponent();
	
	/** Updates BaseHealth based on Armor Protection modifier. */
	void UpdateHealth();
	
	/** Escape button handler */
	void Exit();
	
	/** Destroys character and exits the game */
	void Destroy();

protected:

	/**  Upgrade menu widget class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> UpgradeMenuWidget;
	
	/**  Upgrade menu widget object reference. */
	UUserWidget* UpgradeMenu;
	
	/**  Pause menu widget class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> PauseMenuWidget;
	
	/**  Pause menu widget object reference. */
	UUserWidget* PauseMenu;
	

	/**  Death screen widget class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
	TSubclassOf<class UUserWidget> DeathScreenWidget;

	/**  Death screen widget object reference. */
	UUserWidget* DeathScreen;


	/** Montages to play for attacking. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* AttackLeftMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* AttackRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundCue* AttackSwingCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundCue* AnvilInteractCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundCue* PlayerHurtCue;
	
	/** Sword actor class. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Equipment)
	TSubclassOf<class AActor> SwordActor;
	
	/** Sword actor class. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Equipment)
	TSubclassOf<class AActor> ArmorActor;
	
	/** Called to start and stop sprinting */
    bool sprinting;
    void StartSprinting();
    void StopSprinting();
	
	/** Called for attacking with sword. */
    int lastAttack;
    void Attack();
	
	/** Called for interacting with objects and exiting menus. */
	void Interact();
	
	/** Called for jumping. */
	void StartJumping();
	
	/** Called for yaw input. */
	void Turn(float Value);
	
	/** Called for pitch input. */
	void LookUp(float Value);

    /** Called for forwards/backward input. */
    float moveForwardValue;
    void MoveForward(float Value);

    /** Called for side to side input. */
    float moveRightValue;
    void MoveRight(float Value);

    /** Calculates max walk speed for movement. */
    void CalculateMoveSpeed();
	
	/**  Checks if input is enabled (disabled while a menu is open) */
	bool InputEnabled();
	
	/** Sphere traces for Interactable objects and outlines them */
	AActor* interactable;
	UPrimitiveComponent* interactableOutline;
	void SphereTraceForInteractables();

	/** Removes the controller and ragdolls the player */
	void KillPlayer(float CorpsePersistenceTime);

    virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	FTimerHandle InteractableSphereTraceTimerHandle;
	
	UInputComponent* PlayerInputComponent;
	
	void SetMouseCursor(bool enabled);

	/** Debug function */
	void DebugOne();
};
