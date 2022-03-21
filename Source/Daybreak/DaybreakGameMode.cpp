// Copyright Epic Games, Inc. All Rights Reserved.

#include "DaybreakGameMode.h"
#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
//#include "Kismet/GameplayStatics.h"

ADaybreakGameMode::ADaybreakGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Sword_Base_Character"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADaybreakGameMode::BeginPlay() {
	Super::BeginPlay();
	
	// ADaybreakCharacter* Character = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)); // don't think we actually need this (yet)
	
	// add HUD widget to viewport
	if (HUDWidget != nullptr) {
		UUserWidget* HUD = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);
		if (HUD) {
			HUD->AddToViewport();
		}
	}
}
