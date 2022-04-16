// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DayNightCycle.h"
#include "DaybreakCharacter.h"
#include "DaybreakSword.h"
#include "DaybreakArmor.h"
#include "DaybreakHUD.generated.h"

/**
 * Parent class of Sword_Base_HUD
 */
UCLASS()
class DAYBREAK_API UDaybreakHUD : public UUserWidget {
	GENERATED_BODY()
	
public:
	ADayNightCycle* DayNightController;
	
	UFUNCTION(BlueprintCallable)
	FSwordLevel GetCurrentSwordLevel();
	
	UFUNCTION(BlueprintCallable)
	FArmorLevel GetCurrentArmorLevel();

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnMediaPlayerOpen(FString url);
	
	void SeekFromDegrees(float degrees);
	
	void RefreshMediaPlayer();
	
	int dayLengthSeconds;
	
	void UpdateDayNightIndicator();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Variables)
    class UMediaPlayer* MediaPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Variables)
    class UMediaSource* MediaSource;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Variables)
    FString DayNightText;
	
	ADaybreakCharacter* player;
	ADaybreakSword* sword;
	ADaybreakArmor* armor;
	
	bool mediaPlayerReady;
};
