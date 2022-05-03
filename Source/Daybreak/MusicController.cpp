// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicController.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMusicController::AMusicController()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USoundCue> dayMusicCueObj(TEXT("SoundCue'/Game/Audio/Music/Music_Day_Cue.Music_Day_Cue'"));
	if (dayMusicCueObj.Succeeded()) {
		DayMusicCue = dayMusicCueObj.Object;
		dayMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("DayMusic"));
		dayMusic->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> nightMusicCueObj(TEXT("SoundCue'/Game/Audio/Music/Music_Night_Cue.Music_Night_Cue'"));
	if (nightMusicCueObj.Succeeded()) {
		NightMusicCue = nightMusicCueObj.Object;
		nightMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("NightMusic"));
		nightMusic->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AMusicController::BeginPlay()
{
	Super::BeginPlay();

	// Set audio component sound cues
	if (dayMusic && DayMusicCue) {
		dayMusic->SetSound(DayMusicCue);
	}
	if (nightMusic && NightMusicCue) {
		nightMusic->SetSound(NightMusicCue);
	}

	if (DayNightController) {
		DayNightController->OnDayStart.AddDynamic(this, &AMusicController::OnDayStart);
		DayNightController->OnNightStart.AddDynamic(this, &AMusicController::OnNightStart);
	}
	
}

void AMusicController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (DayNightController->GetIsDay()) {
		if (dayMusic && !dayMusic->IsPlaying()) {
			if (nightMusic) {
				nightMusic->Stop();
			}
			dayMusic->Play(0);
		}
	} else {
		if (nightMusic && !nightMusic->IsPlaying()) {
			if (dayMusic) {
				dayMusic->FadeOut(2, 0);
			}
			nightMusic->FadeIn(2);
		}
	}
}

void AMusicController::OnDayStart(int dayLengthSeconds) {
	UE_LOG(LogActor, Warning, TEXT("OnDayStart in MusicController"));
	if (nightMusic) {
		nightMusic->FadeOut(1, 0);
	}
	if (dayMusic && !dayMusic->IsPlaying()) {
		dayMusic->FadeIn(1);
	}
}

void AMusicController::OnNightStart() {
	UE_LOG(LogActor, Warning, TEXT("OnNightStart in MusicController"));
	if (dayMusic) {
		dayMusic->FadeOut(1, 0);
	}
	if (nightMusic && !nightMusic->IsPlaying()) {
		nightMusic->FadeIn(1);
	}
}
