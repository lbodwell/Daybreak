// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DaybreakSword.h"
#include "DaybreakCharacter.generated.h"

UCLASS(config=Game)
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

public:
    ADaybreakCharacter();

    float BaseTurnRate;
    float BaseLookUpRate;

    /** Whether player is currently attacking and should not be able to attack again yet. */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=State)
	bool Attacking;
	
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
	
	/** Player sword object for blueprints. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=Weapons)
    ADaybreakSword* Sword;
	
	/** Player sword object for C++. */
	ADaybreakSword* GetSword();
	
	UInputComponent* GetPlayerInputComponent();

protected:

	/**  Upgrade menu widget class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> UpgradeMenuWidget;
	
	/**  Upgrade menu widget object reference. */
	UUserWidget* UpgradeMenu;
	
	/** Montages to play for attacking. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* AttackLeftMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* AttackRightMontage;
	
	/** Sword actor class. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapons)
	TSubclassOf<class AActor> SwordActor;
	
	/** Called to start and stop sprinting */
    bool sprinting;
    void StartSprinting();
    void StopSprinting();
	
	/** Called for attacking with sword. */
    int lastAttack;
    void Attack();
	
	/** Called for interacting with objects and exiting menus. */
	void Interact();
	void Exit();
	
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

    virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;
	
	UInputComponent* PlayerInputComponent;
};
