// Copyright Epic Games, Inc. All Rights Reserved.

#include "DaybreakGameMode.h"
#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleResource.h"
#include "Math/UnrealMathUtility.h"

ADaybreakGameMode::ADaybreakGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Sword_Base_Character"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void ShuffleAndRemove(TArray<AActor*> ResourcesToShuffle) {

	// Randomly shuffle arrays
	for (int32 i = ResourcesToShuffle.Num() - 1; i > 0; i--) {
		int32 j = FMath::Floor(FMath::Rand() * (i + 1)) % ResourcesToShuffle.Num();
		AActor* temp = ResourcesToShuffle[i];
		ResourcesToShuffle[i] = ResourcesToShuffle[j];
		ResourcesToShuffle[j] = temp;
	}

	// "Remove" the first half (number can be adjusted) of the Resources in the TArray
	for (int i = 0; i < ResourcesToShuffle.Num() / 2; i++) {
		ResourcesToShuffle[i]->SetActorHiddenInGame(true);
		ResourcesToShuffle[i]->SetActorEnableCollision(false);
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

	// Find all Resources and compile them into a TArray
	TArray<AActor*> AllResources;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleResource::StaticClass(), AllResources);

	TArray<AActor*> ResourcesLevel1;
	TArray<AActor*> ResourcesLevel2;
	TArray<AActor*> ResourcesLevel3;

	// Separate all resources based on level
	for (int32 i = 0; i < AllResources.Num(); i++) {
		ADestructibleResource* Resource = Cast<ADestructibleResource>(AllResources[i]);

		if (Resource->Level == 1) {
			ResourcesLevel1.Add(AllResources[i]);
		}

		if (Resource->Level == 2) {
			ResourcesLevel2.Add(AllResources[i]);
		}

		if (Resource->Level == 3) {
			ResourcesLevel3.Add(AllResources[i]);
		}
	}

	// Shuffle Resources by level & Remove Some of Each Set
	ShuffleAndRemove(ResourcesLevel1);
	ShuffleAndRemove(ResourcesLevel2);
	ShuffleAndRemove(ResourcesLevel3);
}

void ADaybreakGameMode::DamagePortal(int DamageAmount) {
	PortalHealth -= DamageAmount;

	if (PortalHealth <= 0) {
		//kill player
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->Destroy();
	}
}
