// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DayNightCycle.h"
#include "DaybreakHUD.generated.h"

/**
 * Parent class of Sword_Base_HUD
 */
UCLASS()
class DAYBREAK_API UDaybreakHUD : public UUserWidget {
	GENERATED_BODY()
	
public:
	ADayNightCycle* DayNightController;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnMediaPlayerOpen(FString url);
	
	void SeekFromDegrees(float degrees);
	
	void UpdateDayNightIndicator();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Variables)
    class UMediaPlayer* MediaPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Variables)
    class UMediaSource* MediaSource;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Variables)
    FString DayNightText;
	
	bool mediaPlayerReady;
	
	float lastRotation;
	float lastRotationTime;
	float rateOfRotation;
	
	void TrackRateOfRotation();
};