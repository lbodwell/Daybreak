// Copyright Epic Games, Inc. All Rights Reserved.

#include "DaybreakCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////
// ADaybreakCharacter

ADaybreakCharacter::ADaybreakCharacter() {
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    sprinting = false;
    Attacking = false;
    lastAttack = 1;
	TurningDirection = 0;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 300;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADaybreakCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ADaybreakCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ADaybreakCharacter::MoveRight);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADaybreakCharacter::StartSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADaybreakCharacter::StopSprinting);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &ADaybreakCharacter::Turn);
    PlayerInputComponent->BindAxis("TurnRate", this, &ADaybreakCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &ADaybreakCharacter::LookUpAtRate);

    // combat
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ADaybreakCharacter::Attack);
}

void ADaybreakCharacter::Turn(float Value) {
    AddControllerYawInput(Value);
	TurningSpeed = Value;
}

void ADaybreakCharacter::TurnAtRate(float Rate) {
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADaybreakCharacter::LookUpAtRate(float Rate) {
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADaybreakCharacter::MoveForward(float Value) {
    moveForwardValue = Value;
    if (Controller != nullptr && Value != 0.0f) {
        CalculateMoveSpeed();

        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ADaybreakCharacter::MoveRight(float Value) {
    moveRightValue = Value;
    if (Controller != nullptr && Value != 0.0f) {
        CalculateMoveSpeed();

        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ADaybreakCharacter::CalculateMoveSpeed() {
    float speed = 300;
    if (sprinting) {
        speed *= 2; // double sprinting speed
    }
    if (moveForwardValue != 0 && moveRightValue != 0) {
        speed *= 0.75; // 3/4 diagonal speed
    }
    GetCharacterMovement()->MaxWalkSpeed = moveForwardValue < 0.0f ? 150 : speed; // backward speed = 150
}

void ADaybreakCharacter::StartSprinting() {
    sprinting = true;
}

void ADaybreakCharacter::StopSprinting() {
    sprinting = false;
}

void ADaybreakCharacter::Attack() {
	if (AttackLeftMontage && AttackRightMontage && !Attacking) {
		float const duration = PlayAnimMontage(lastAttack == 0 ? AttackRightMontage : AttackLeftMontage, 1, NAME_None);
        if (duration > 0.f) {
            Attacking = true; // will be unset by AnimNotify::AttackHitEnd in AnimBP
            lastAttack = lastAttack == 0 ? 1 : 0; // alternate between left and right attacks
        }
	}
}
