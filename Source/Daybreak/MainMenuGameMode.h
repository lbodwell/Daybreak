// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraActor.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DAYBREAK_API AMainMenuGameMode : public AGameModeBase {
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
public:
	AMainMenuGameMode();
	
	/**  Main menu widget class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> MainMenuWidget;
};
