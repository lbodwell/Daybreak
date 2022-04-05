// Copyright Epic Games, Inc. All Rights Reserved.

#include "DaybreakGameMode.h"
#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "DestructibleResource.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

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

	// Find all Resources and compile them into a TArray
	TArray<AActor*> FoundResources;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleResource::StaticClass(), FoundResources);

	// Randomly shuffle array
	for (int32 i = FoundResources.Num() - 1; i > 0; i--) {
		int32 j = FMath::Floor(FMath::Rand() * (i + 1)) % FoundResources.Num();
		AActor* temp = FoundResources[i];
		FoundResources[i] = FoundResources[j];
		FoundResources[j] = temp;
	}

	// "Remove" the first half (number can be adjusted) of the Resources in the TArray
	for (int i = 0; i < FoundResources.Num()/2; i++) {
		FoundResources[i]->SetActorHiddenInGame(true);
		FoundResources[i]->SetActorEnableCollision(false);
	}
}
