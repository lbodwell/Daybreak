// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DaybreakCharacter.h"
#include "DaybreakHUD.h"
#include "DaybreakGameMode.generated.h"

UCLASS(minimalapi)
class ADaybreakGameMode : public AGameModeBase {
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
private:
	void TriggerDarkstoneTutorial();

public:
	ADaybreakGameMode();
	
	/** Player base health at full. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
    float BasePortalHealth;
	
	/** Player health currently. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=State)
    float PortalHealth;

	UFUNCTION(BlueprintCallable)
	void DamagePortal(int DamageAmount);
	
	float GetDistanceToPlayer(FVector point);
	float GetDistanceToPortal(FVector point);
	
	UDaybreakHUD* GetHUD();
	
	static int EnemyCount;
	static float EnemyValue;

protected:
	/**  HUD widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
    TSubclassOf<class UUserWidget> HUDWidget;
	
	UDaybreakHUD* HUD;
	
	AActor* portal;
	AActor* tutorialDarkstone;
	
	ADaybreakCharacter* player;
};



