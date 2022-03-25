// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include <math.h> 
#include <string>

bool UDaybreakHUD::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;
	
	mediaPlayerReady = false;
	
	lastRotation = -1;
	lastRotationTime = -1;
	rateOfRotation = 0;
	
	return true;
}

void UDaybreakHUD::NativeConstruct() {
	Super::NativeConstruct();
	
	// get DayNightController in world
	DayNightController = Cast<ADayNightCycle>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayNightCycle::StaticClass()));
	
	// open media source for day/night rotator
	MediaPlayer->OnMediaOpened.AddDynamic(this, &UDaybreakHUD::OnMediaPlayerOpen);
	MediaPlayer->OpenSource(MediaSource);
		
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UDaybreakHUD::TrackRateOfRotation, 0.05, true);
	
	FTimerHandle timerHandle2;
	GetWorld()->GetTimerManager().SetTimer(timerHandle2, this, &UDaybreakHUD::UpdateDayNightIndicator, 1, true);
}

void UDaybreakHUD::OnMediaPlayerOpen(FString url) {
	mediaPlayerReady = true;
	MediaPlayer->Pause();
	SeekFromDegrees(DayNightController->CurrentRotation);
}

// seek day/night rotator media to correct timestamp based on degree rotation (0 - 180 = night, 180 - 360 = night)
void UDaybreakHUD::SeekFromDegrees(float degrees) {
	if (!mediaPlayerReady) return;
	const float frames = degrees / 360 * 1800;
	const int seconds = floor(frames / 60);
	const int milliseconds = floor((frames - (seconds * 60)) * 16.667);
	MediaPlayer ->Seek(FTimespan(0, 0, 0, seconds, milliseconds));
}

// track rate of rotation of DayNightController to update day/night rotator
void UDaybreakHUD::TrackRateOfRotation() {
	const float currentRotation = DayNightController->CurrentRotation;
	
	if (lastRotation != currentRotation) {
		SeekFromDegrees(currentRotation);
		const float currentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		
		// compute rate of rotation based on lastRotation and lastRotationTime
		if (lastRotation > -1 && currentTime > 1) {
			rateOfRotation = (currentRotation - lastRotation) / (currentTime - lastRotationTime);
		}
		
		lastRotation = currentRotation;
		lastRotationTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	}
}

// update day/night rotator text
void UDaybreakHUD::UpdateDayNightIndicator() {
	const float currentRotation = DayNightController->CurrentRotation;
	const float currentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	
	// estimated daylight remaining
	if (currentRotation >= 180 && rateOfRotation > 0) {
		const float timeEstimate = ((360 - lastRotation) / rateOfRotation) - (currentTime - lastRotationTime);
		const int minutes = floor(timeEstimate / 60);
		const int seconds = floor(timeEstimate - (minutes * 60));
		DayNightText = FString::FromInt(minutes) + (seconds < 10 ? ":0" : ":") + FString::FromInt(seconds);
	}
	// enemy count remaining for nighttime
	else {
		DayNightText = FString(TEXT("0"));
	}
}