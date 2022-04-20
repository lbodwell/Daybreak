// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"
#include "Components/SkyLightComponent.h"
#include <cmath>

// Sets default values
ADayNightCycle::ADayNightCycle() {
	PrimaryActorTick.bCanEverTick = false;
	
	CurrentRotation = 270; // rotation begins with Sun at it's highest point (dictated by its actual starting rotation in the world)
	
	tickRate = 0.01; // update sky rotation 100 times every second
}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay() {
	Super::BeginPlay();

	SetRotation(170); // begin the game at sunrise
	
	tickRotation = 180 / (DayLengthSeconds * (1 / tickRate)); // calculate sky rotation per tick
	
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADayNightCycle::UpdateRotation, tickRate, true);
}

void ADayNightCycle::UpdateRotation() {
	if (Sun != nullptr && Moon != nullptr && SkyLight != nullptr) {
		float newRotation = CurrentRotation + tickRotation;
		if (newRotation >= 360) {
			newRotation -= 360;
			GetWorld()->GetTimerManager().ClearTimer(timerHandle);
			UE_LOG(LogActor, Warning, TEXT("It should be night now"));
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
					OnNightStart.Broadcast();
				}, 1, false);
		} else if (CurrentRotation < 180 && newRotation >= 180) {
			OnDayStart.Broadcast(DayLengthSeconds);
			UE_LOG(LogActor, Warning, TEXT("It should be day now"));
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADayNightCycle::UpdateRotation, tickRate, true);
		}

		SetRotation(newRotation);
	}
}

void ADayNightCycle::AddRotation(float Angle) {
	if (Sun != nullptr && Moon != nullptr && SkyLight != nullptr && Angle > 0 && Angle < 360) {
		float newRotation = CurrentRotation + Angle;
		if (newRotation >= 360) {
			newRotation -= 360;
			GetWorld()->GetTimerManager().ClearTimer(timerHandle);
			UE_LOG(LogActor, Warning, TEXT("It should be night now"));
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
					OnNightStart.Broadcast();
				}, 1, false);
		} else if (CurrentRotation < 180 && newRotation >= 180) {
			OnDayStart.Broadcast(DayLengthSeconds);
			UE_LOG(LogActor, Warning, TEXT("It should be day now"));
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADayNightCycle::UpdateRotation, tickRate, true);
		}

		SetRotation(newRotation);

		CurrentRotation = newRotation;
	}
}

void ADayNightCycle::SetRotation(float Angle) {
	if (Sun != nullptr && Moon != nullptr && SkyLight != nullptr && Angle >= 0 && Angle <=360) {
		float rotation = Angle - CurrentRotation;
		
		// change in rotation must be at least 0.1 to turn from turn over from -90 to 90
		if (std::abs(rotation) < 0.1 && Sun->GetActorRotation().Pitch == -90) {
			rotation = 0.1;
		}
		
		Sun->AddActorLocalRotation(FRotator(rotation, 0, 0));
		Moon->AddActorLocalRotation(FRotator(rotation, 0, 0));
		SkyLight->GetLightComponent()->RecaptureSky();
		
		CurrentRotation = Angle;
	}
}

float ADayNightCycle::GetDayLengthSecondsRemaining() {
	if (CurrentRotation > 180) {
		return (360 - CurrentRotation) / (tickRotation * (1 / tickRate));
	} else {
		return 0;
	}
}

int ADayNightCycle::GetDayLengthSeconds() {
	return DayLengthSeconds;
}

bool ADayNightCycle::GetIsDay() {
	return (GetDayLengthSecondsRemaining() > 0);
}