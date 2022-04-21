// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"

#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

AMainMenuGameMode::AMainMenuGameMode() {
}

void AMainMenuGameMode::BeginPlay() {
	Super::BeginPlay();
	
	// add widget
	if (MainMenuWidget) {
		UUserWidget* MainMenu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidget);
		if (MainMenu) {
			MainMenu->AddToViewport();
		}
	}
	
	APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	// enable mouse
	playerController->bShowMouseCursor = true;
	playerController->bEnableClickEvents = true;
	playerController->bEnableMouseOverEvents = true;
	
	FInputModeUIOnly inputMode;
	playerController->SetInputMode(inputMode);
	
	// set camera
	TArray<AActor*> cameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), cameraActors);
	playerController->SetViewTarget(cameraActors[0]);
}
