// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include <math.h> 
#include <string>
#include "DaybreakCharacter.h"
#include "DaybreakSword.h"

bool UDaybreakHUD::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;
	
	mediaPlayerReady = false;
	
	return true;
}

void UDaybreakHUD::NativeConstruct() {
	Super::NativeConstruct();
	
	// get DayNightController in world
	DayNightController = Cast<ADayNightCycle>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayNightCycle::StaticClass()));
	
	// open media source for day/night rotator
	MediaPlayer->OnMediaOpened.AddDynamic(this, &UDaybreakHUD::OnMediaPlayerOpen);
	MediaPlayer->OpenSource(MediaSource);
	
	player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	// update day/night indicator once a second
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

// update day/night rotator text
void UDaybreakHUD::UpdateDayNightIndicator() { 
	SeekFromDegrees(DayNightController->CurrentRotation);
	
	// display minutes:seconds of daylight remaining
	if (DayNightController->CurrentRotation >= 180) {
		float seconds = DayNightController->GetDayLengthSecondsRemaining();
		float minutes = seconds / 60;
		DayNightText = FString::FromInt(minutes) + (seconds < 10 ? ":0" : ":") + FString::FromInt(seconds);
	}
	// display enemy count remaining for nighttime
	else {
		DayNightText = FString(TEXT("0"));
	}
}

FSwordLevel UDaybreakHUD::GetCurrentSwordLevel() {
	if (sword == nullptr) {
		sword = player->GetSword();
	}
	
	if (sword != nullptr) {
		return sword->CurrentLevel;
	}
	return FSwordLevel();
}
