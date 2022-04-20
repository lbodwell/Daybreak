// Copyright Epic Games, Inc. All Rights Reserved.

#include "DaybreakGameMode.h"
#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ADaybreakGameMode::ADaybreakGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Sword_Base_Character"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADaybreakGameMode::BeginPlay() {
	Super::BeginPlay();
	
	// add HUD widget to viewport
	if (HUDWidget != nullptr) {
		UUserWidget* HUD = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);
		if (HUD) {
			HUD->AddToViewport();
		}
	}
	
	APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	// disable mouse
	playerController->bShowMouseCursor = false;
	playerController->bEnableClickEvents = false;
	playerController->bEnableMouseOverEvents = false;
	
	FInputModeGameOnly inputMode;
	playerController->SetInputMode(inputMode);
}

void ADaybreakGameMode::DamagePortal(int DamageAmount) {
	PortalHealth -= DamageAmount;
}
