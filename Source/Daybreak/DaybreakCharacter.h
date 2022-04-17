// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DaybreakSword.h"
#include "DaybreakArmor.h"
#include "GameFramework/PlayerController.h"
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
	int DayCount;
	
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

protected:

	/**  Upgrade menu widget class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> UpgradeMenuWidget;
	
	/**  Upgrade menu widget object reference. */
	UUserWidget* UpgradeMenu;
	
	/**  Pause menu widget class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> PauseMenuWidget;
	
	/**  Upgrade menu widget object reference. */
	UUserWidget* PauseMenu;
	
	/** Montages to play for attacking. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* AttackLeftMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* AttackRightMontage;
	
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

	/** Destroys character and exits the game */
	void Destroy();

    virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	FTimerHandle InteractableSphereTraceTimerHandle;
	
	UInputComponent* PlayerInputComponent;
	
	void SetMouseCursor(bool enabled);

};
