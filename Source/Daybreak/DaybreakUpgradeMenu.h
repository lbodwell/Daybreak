// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DaybreakSword.h"
#include "DaybreakCharacter.h"
#include "DaybreakUpgradeMenu.generated.h"

/**
 * 
 */
UCLASS()
class DAYBREAK_API UDaybreakUpgradeMenu : public UUserWidget {
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintCallable)
		FSwordLevel GetCurrentSwordLevel();
		
		UFUNCTION(BlueprintCallable)
		FSwordLevel GetNextSwordLevel();
		
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUI();
	
	protected:
		virtual bool Initialize() override;
		virtual void NativeConstruct() override;
		virtual void NativeDestruct() override;
		
		ADaybreakCharacter* player;
		ADaybreakSword* sword;
		
		TArray<AActor*> SwordCameras;
		
		bool isUpgrading;
		FTimerHandle upgradeTimerHandle;
		
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float UpgradeProgress;
		
		void StartUpgrading();
		void StopUpgrading();
		void Upgrade();
};
