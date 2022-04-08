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
	
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADayNightCycle::UpdateRotation, tickRate, true);
}

void ADayNightCycle::UpdateRotation() {
	if (Moon != nullptr && Sun != nullptr && SkyLight != nullptr) {
		float newRotation = CurrentRotation + tickRotation;
		if (newRotation >= 360) {
			newRotation -= 360;
		} else if (CurrentRotation < 180 && newRotation >= 180) {
			OnDayStart.Broadcast(DayLengthSeconds);
		}
		
		SetRotation(newRotation);
	}
}

void ADayNightCycle::SetRotation(float newRotation) {
	if (Sun != nullptr && Moon != nullptr && SkyLight != nullptr && newRotation >= 0 && newRotation <=360) {
		float rotation = newRotation - CurrentRotation;
		
		// change in rotation must be at least 0.1 to turn from turn over from -90 to 90
		if (std::abs(rotation) < 0.1 && Sun->GetActorRotation().Pitch == -90) {
			rotation = 0.1;
		}
		
		Sun->AddActorLocalRotation(FRotator(rotation, 0, 0));
		Moon->AddActorLocalRotation(FRotator(rotation, 0, 0));
		SkyLight->GetLightComponent()->RecaptureSky();
		
		CurrentRotation = newRotation;
	}
}

float ADayNightCycle::GetDayLengthSecondsRemaining() {
	float dayEnd = CurrentRotation <= 180 ? 180 : 360;
	float remainingRotation = dayEnd - CurrentRotation;
	
	return remainingRotation / (tickRotation * (1 / tickRate));
}

int ADayNightCycle::GetDayLengthSeconds() {
	return DayLengthSeconds;
}