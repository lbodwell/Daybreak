// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DaybreakGameMode.generated.h"

UCLASS(minimalapi)
class ADaybreakGameMode : public AGameModeBase {
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	ADaybreakGameMode();

protected:
	/**  HUD widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> HUDWidget;
};



