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
#include "DaybreakGameMode.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"

ADaybreakCharacter::ADaybreakCharacter() {
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;
	
    sprinting = false;
    Attacking = false;
    lastAttack = 1;
	TurningVelocity = 0;
	BaseHealth = 100;
	Health = BaseHealth;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
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

	// Initialize audio components
	static ConstructorHelpers::FObjectFinder<USoundCue> attackSwingCueObj(TEXT("SoundCue'/Game/Audio/Player/Attack/Player_Attack_Swing_Cue.Player_Attack_Swing_Cue'"));
	if (attackSwingCueObj.Succeeded()) {
		AttackSwingCue = attackSwingCueObj.Object;
		attackSwingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSwingSound"));
		attackSwingSound->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> anvilInteractCueObj(TEXT("SoundCue'/Game/Audio/Player/UI/Anvil/UI_Anvil_Interact_Cue.UI_Anvil_Interact_Cue'"));
	if (anvilInteractCueObj.Succeeded()) {
		AnvilInteractCue = anvilInteractCueObj.Object;
		anvilInteractSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AnvilInteractSound"));
		anvilInteractSound->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> playerHurtCueObj(TEXT("SoundCue'/Game/Audio/Player/Hurt/Player_Hurt_Cue.Player_Hurt_Cue'"));
	if (playerHurtCueObj.Succeeded()) {
		PlayerHurtCue = playerHurtCueObj.Object;
		playerHurtSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerHurtSound"));
		playerHurtSound->SetupAttachment(RootComponent);
	}
}

void ADaybreakCharacter::BeginPlay() {
    Super::BeginPlay();
	
	// get player controller
	playerController = Cast<APlayerController>(GetController());
	
	// create sword and attach to WeaponSocket
	FVector socketLocation = GetMesh()->GetSocketLocation(FName(TEXT("WeaponSocket")));
	Sword = GetWorld()->SpawnActor<ADaybreakSword>(SwordActor, socketLocation, socketLocation.Rotation());
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("WeaponSocket")));
	
	// create armor
	Armor = GetWorld()->SpawnActor<ADaybreakArmor>(ArmorActor, FVector(0, 0, 0), FRotator(0, 0, 0));
	
	// start sphere tracing for interactables
	GetWorld()->GetTimerManager().SetTimer(InteractableSphereTraceTimerHandle, this, &ADaybreakCharacter::SphereTraceForInteractables, 0.25, true);

	// Set audio component sound cues
	if (attackSwingSound && AttackSwingCue) {
		attackSwingSound->SetSound(AttackSwingCue);
	}
	if (anvilInteractSound && AnvilInteractCue) {
		anvilInteractSound->SetSound(AnvilInteractCue);
	}
	if (playerHurtSound && PlayerHurtCue) {
		playerHurtSound->SetSound(PlayerHurtCue);
	}
}

// Called ever frame
void ADaybreakCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// This can probably be event-based and not happen on every tick
	if (Sword != nullptr) {
		Sword->Hitting = Hitting;
	}
}

// --- MOVEMENT INPUT --- //

void ADaybreakCharacter::SetupPlayerInputComponent(class UInputComponent* playerInputComponent) {
	PlayerInputComponent = playerInputComponent;
	
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADaybreakCharacter::StartJumping);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ADaybreakCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ADaybreakCharacter::MoveRight);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADaybreakCharacter::StartSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADaybreakCharacter::StopSprinting);
	
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADaybreakCharacter::Interact);
	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &ADaybreakCharacter::Exit);

    PlayerInputComponent->BindAxis("Turn", this, &ADaybreakCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ADaybreakCharacter::LookUp);

    // combat
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ADaybreakCharacter::Attack);
	
	// debug
	//PlayerInputComponent->BindAction("Debug1", IE_Pressed, this, &ADaybreakCharacter::DebugOne);
}

void ADaybreakCharacter::StartJumping() {
	if (InputEnabled()) {
		GetCharacterMovement()->JumpZVelocity = 600 + (150 * Armor->CurrentLevel.MovementSpeed);
		Jump();
	}
}

void ADaybreakCharacter::Turn(float Value) {
	if (InputEnabled()) {
		AddControllerYawInput(Value);
		TurningVelocity = Value;
	} else {
		TurningVelocity = 0;
	}
}

void ADaybreakCharacter::LookUp(float Value) {
	if (InputEnabled()) {
		AddControllerPitchInput(Value);
	}
}

void ADaybreakCharacter::MoveForward(float Value) {
	if (InputEnabled()) {
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
}

void ADaybreakCharacter::MoveRight(float Value) {
	if (InputEnabled()) {
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
}

void ADaybreakCharacter::CalculateMoveSpeed() {
    float speed = 300;
    if (sprinting) {
        speed *= 2; // double sprinting speed
    }
    if (moveForwardValue != 0 && moveRightValue != 0) {
        speed *= 0.75; // 3/4 diagonal speed
    }
	speed = moveForwardValue < 0.0f ? 225 : speed; // backward speed = 225
	speed = speed + (speed / 4 * Armor->CurrentLevel.MovementSpeed); // add Armor MovementSpeed modifier;
	
    GetCharacterMovement()->MaxWalkSpeed = speed; // set MaxWalkSpeed
}

void ADaybreakCharacter::StartSprinting() {
    sprinting = true;
}

void ADaybreakCharacter::StopSprinting() {
    sprinting = false;
}

// --- ACTION INPUT --- //

void ADaybreakCharacter::Attack() {
	if (InputEnabled() && AttackLeftMontage && AttackRightMontage && !Attacking) {
		float const duration = PlayAnimMontage(lastAttack == 0 ? AttackRightMontage : AttackLeftMontage, 1, NAME_None);
        if (duration > 0.f) {
            Attacking = true; // will be unset by AnimNotify::AttackHitEnd in AnimBP
            lastAttack = lastAttack == 0 ? 1 : 0; // alternate between left and right attacks
			
			float attackDelay = 0.9 - (Armor->CurrentLevel.AttackSpeed * 0.25); // set attack delay based on Armor AttackSpeed modifier
			FTimerHandle timerHandle;
			GetWorld()->GetTimerManager().SetTimer(timerHandle, [&]() { Attacking = false; }, 1, false, attackDelay);
			if (attackSwingSound) {
				attackSwingSound->Play(0);
			}
        }
	}
}

void ADaybreakCharacter::Interact() {
	if (InputEnabled() && interactable) {
		//UE_LOG(LogActor, Warning, TEXT("Interactable: %s"), *(interactable->GetName()));
		
		if (interactable->ActorHasTag("Anvil")) {
			// add upgrade menu widget to viewport
			if (UpgradeMenuWidget != nullptr) {
				UpgradeMenu = CreateWidget<UUserWidget>(GetWorld(), UpgradeMenuWidget);
				if (UpgradeMenu) {
					UpgradeMenu->AddToViewport();
					if (anvilInteractSound && !anvilInteractSound->IsPlaying()) {
						anvilInteractSound->Play(0);
					}
				}
			}
		}
	}
}

void ADaybreakCharacter::Exit() {
	if (UpgradeMenu) {
		UpgradeMenu->RemoveFromViewport();
		UpgradeMenu = nullptr;
	}
	else if (PauseMenu) {
		PauseMenu->RemoveFromViewport();
		PauseMenu = nullptr;
		SetMouseCursor(false);
	} 
	else if (!DeathScreen) {
		if (PauseMenuWidget != nullptr) {
			PauseMenu = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidget);
			if (PauseMenu) {
				PauseMenu->AddToViewport();
				SetMouseCursor(true);
			}
		}
	}
}

void ADaybreakCharacter::ReceiveDamage(int amount) {
	Health -= amount;
	
	if (Health <= 0) {
		Health = 0;
		KillPlayer(0.2);
		// death sound
	}
	
	if (playerHurtSound && !playerHurtSound->IsPlaying()) {
		playerHurtSound->Play(0);
	}
}

void ADaybreakCharacter::KillPlayer(float CorpsePersistenceTime) {
	GetWorldTimerManager().ClearTimer(InteractableSphereTraceTimerHandle);

	//This makes the player fall throught the world for some reason
	/*
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetController()->UnPossess();
	
	GetMesh()->SetSimulatePhysics(true);
	*/

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADaybreakCharacter::Destroy, 0.1, false, CorpsePersistenceTime);	
}

void ADaybreakCharacter::Destroy() {
	//	UKismetSystemLibrary::QuitGame(GetWorld(), Cast<APlayerController>(GetController()), EQuitPreference::Type::Quit, false);
	if (DeathScreenWidget != nullptr) {
		DeathScreen = CreateWidget<UUserWidget>(GetWorld(), DeathScreenWidget);
		if (DeathScreen) {
			DeathScreen->AddToViewport();
			SetMouseCursor(true);
		}
	}
}


// --- TIMERS --- //

void ADaybreakCharacter::SphereTraceForInteractables() {
	// get viewpoint
	FVector viewpointLocation;
	FRotator viewpointRotation;
	GetController()->GetPlayerViewPoint(viewpointLocation, viewpointRotation);
	
	// start and end of sphere trace
	FVector start = viewpointLocation;
	FVector end = start + (viewpointRotation.Vector() * 750);
	
	// sphere shape and rotation
	FCollisionShape sphere = FCollisionShape::MakeSphere(75);
	FQuat sphereRotation = FQuat::Identity;
	
	// trace for Interactable (ECC_GameTraceChannel1) object types
	FCollisionQueryParams traceParams;
	FHitResult hitResult;
	bool hit = GetWorld()->SweepSingleByObjectType(hitResult, start, end, sphereRotation, ECC_GameTraceChannel1, sphere, traceParams);
	
	AActor* actor = hit ? hitResult.GetActor() : nullptr;
	bool newInteractable = hit && (interactable == nullptr || actor->GetName() != interactable->GetName());
	
	// check if existing interactable outline should be removed
	if (interactable != nullptr && (newInteractable || !hit)) {
		interactable = nullptr;
		
		if (interactableOutline != nullptr) {
			interactableOutline->SetRenderCustomDepth(false);
			interactableOutline = nullptr;
		}
	}
	
	// check if new interactable should be outlined
	if (newInteractable) {
		interactable = actor;
		
		// outline actor's static mesh component tagged with "Outline"
		TArray<UActorComponent*> outlines = interactable->GetComponentsByTag(UStaticMeshComponent::StaticClass(), "Outline");
		if (outlines[0] != nullptr) {
			interactableOutline = Cast<UPrimitiveComponent>(outlines[0]);
			interactableOutline->SetRenderCustomDepth(true);
		}
	}
}

// --- HELPERS --- //

ADaybreakSword* ADaybreakCharacter::GetSword() {
	return Sword;
}

ADaybreakArmor* ADaybreakCharacter::GetArmor() {
	return Armor;
}

bool ADaybreakCharacter::InputEnabled() {
	return UpgradeMenu == nullptr && PauseMenu == nullptr && DeathScreen == nullptr;
}

UInputComponent* ADaybreakCharacter::GetPlayerInputComponent() {
	return PlayerInputComponent;
}

void ADaybreakCharacter::UpdateHealth() {
	float percentage = Health / BaseHealth;
	BaseHealth = 100 + 100 * Armor->CurrentLevel.Protection;
	Health = BaseHealth * percentage;
}

void ADaybreakCharacter::SetMouseCursor(bool enabled) {
	playerController->bShowMouseCursor = enabled;
	playerController->bEnableClickEvents = enabled;
	playerController->bEnableMouseOverEvents = enabled;
	
	if (enabled) {
		FInputModeGameAndUI inputMode;
		playerController->SetInputMode(inputMode);
	} else {
		FInputModeGameOnly inputMode;
		playerController->SetInputMode(inputMode);
	}
}


void ADaybreakCharacter::DebugOne() {
	DarkStone += 100;
}
