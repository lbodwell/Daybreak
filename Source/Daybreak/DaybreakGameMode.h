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
	
	/** Player base health at full. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
    float BasePortalHealth;
	
	/** Player health currently. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
    float PortalHealth;

protected:
	/**  HUD widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> HUDWidget;
};



