// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include <math.h> 
#include <string>
#include "DaybreakCharacter.h"
#include "DaybreakSword.h"
#include "DaybreakArmor.h"
#include "DaybreakGameMode.h"


bool UDaybreakHUD::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;
	
	mediaPlayerReady = false;
	messageShown = false;
	
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
	GetWorld()->GetTimerManager().SetTimer(timerHandle2, this, &UDaybreakHUD::UpdateDayNightIndicator, 0.1, true);
}

void UDaybreakHUD::OnMediaPlayerOpen(FString url) {
	MediaPlayer->SetLooping(true);
	mediaPlayerReady = true;
	RefreshMediaPlayer();
}

// updates media player to be at the right rotation with the right play rate
void UDaybreakHUD::RefreshMediaPlayer() {
	if (mediaPlayerReady && DayNightController) {
		SeekFromDegrees(DayNightController->CurrentRotation);
		dayLengthSeconds = DayNightController->GetDayLengthSeconds();
		MediaPlayer->SetRate(15 / (float)dayLengthSeconds); // video spans 30 seconds (15 second day/night)
	}
}

// seek day/night rotator media to correct timestamp based on degree rotation (0 - 180 = night, 180 - 360 = night)
void UDaybreakHUD::SeekFromDegrees(float degrees) {
	if (!mediaPlayerReady) return;
	const float frames = degrees / 360 * 1800; // video contains 1800 frames (30 seconds @ 60fps)
	const int seconds = floor(frames / 60);
	const int milliseconds = floor((frames - (seconds * 60)) * 16.667);
	MediaPlayer->Seek(FTimespan(0, 0, 0, seconds, milliseconds));
}

// update day/night rotator text
void UDaybreakHUD::UpdateDayNightIndicator() { 
	if (DayNightController) {
		// display minutes:seconds of daylight remaining
		if (DayNightController->CurrentRotation > 180) {
			// check if day length changes
			if (dayLengthSeconds != DayNightController->GetDayLengthSeconds()) {
				RefreshMediaPlayer();
			}
			
			int totalSeconds = DayNightController->GetDayLengthSecondsRemaining();
			int minutes = floor((float)totalSeconds / 60);
			int seconds = totalSeconds - minutes * 60;
			DayNightText = FString::FromInt(minutes) + (seconds < 10 ? ":0" : ":") + FString::FromInt(seconds);
		}
		// display enemy count remaining for nighttime
		else {
			if (MediaPlayer->GetRate() != 0) {
				MediaPlayer->Pause();
			}
			SeekFromDegrees(DayNightController->CurrentRotation);
			
			DayNightText = FString::FromInt(ADaybreakGameMode::EnemyCount);
		}
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

FArmorLevel UDaybreakHUD::GetCurrentArmorLevel() {
	if (armor == nullptr) {
		armor = player->GetArmor();
	}
	
	if (armor != nullptr) {
		return armor->CurrentLevel;
	}
	return FArmorLevel();
}

void UDaybreakHUD::AddMessage(FString message) {
	if (messageShown) {
		RemoveMessage();
		
		nextMessage = message;
		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, [&]{ AddMessage(nextMessage); }, 1.25, false);
	} else {
		messageShown = true;
		ShowMessage(message);
	}
}

void UDaybreakHUD::RemoveMessage() {
	messageShown = false;
	HideMessage();
}

void UDaybreakHUD::ShowMessage_Implementation(const FString& message) {}

void UDaybreakHUD::HideMessage_Implementation() {}