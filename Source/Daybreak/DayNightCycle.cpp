// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"
#include "Components/SkyLightComponent.h"
#include <cmath>
#include "Engine.h"
#include "DaybreakCharacter.h"

// Sets default values
ADayNightCycle::ADayNightCycle() {
	PrimaryActorTick.bCanEverTick = false;
	
	CurrentRotation = 270; // rotation begins with Sun at it's highest point (dictated by its actual starting rotation in the world)
	
	tickRate = 0.01; // update sky rotation 100 times every second
}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay() {
	Super::BeginPlay();

	SetRotation(180); // begin the game at sunrise
	
	// broadcast first OnDayStart in 1 second
	FTimerHandle broadcastTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(broadcastTimerHandle, [&]() { 
		OnDayStart.Broadcast(DayLengthSeconds); 
		UE_LOG(LogActor, Warning, TEXT("OnDayStart")); 
	}, 1, false);
	
	tickRotation = 180 / (DayLengthSeconds * (1 / tickRate)); // calculate sky rotation per tick
	
	BeginAutoRotation();
}

void ADayNightCycle::BeginAutoRotation() {
	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, FName("AddRotation"), tickRotation);
	GetWorld()->GetTimerManager().SetTimer(autoRotateTimerHandle, timerDelegate, tickRate, true);
}

void ADayNightCycle::AddRotation(float angle) {
	if (Sun != nullptr && Moon != nullptr && SkyLight != nullptr && angle > 0 && angle < 360) {
		float newRotation = CurrentRotation + angle;
		if (newRotation >= 360) {
			newRotation -= 360;
			GetWorld()->GetTimerManager().ClearTimer(autoRotateTimerHandle);
			
			// broadcast OnNightStart in 1 second
			FTimerHandle broadcastTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(broadcastTimerHandle, [&]() { 
				OnNightStart.Broadcast(); 
				UE_LOG(LogActor, Warning, TEXT("OnNightStart"));
			}, 1, false);
		} else if (CurrentRotation < 180 && newRotation >= 180) {
			BeginAutoRotation();

			ADaybreakCharacter* player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			player->DayCount += 1;
			
			// broadcast OnDayStart in 1 second
			FTimerHandle broadcastTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(broadcastTimerHandle, [&]() { 
				OnDayStart.Broadcast(DayLengthSeconds); 
				UE_LOG(LogActor, Warning, TEXT("OnDayStart"));
			}, 1, false);
		}

		SetRotation(newRotation);
	}
}

void ADayNightCycle::SetRotation(float angle) {
	if (Sun != nullptr && Moon != nullptr && SkyLight != nullptr && angle >= 0 && angle <=360) {
		float rotation = angle - CurrentRotation;
		
		// change in rotation must be at least 0.1 to turn from turn over from -90 to 90
		if (std::abs(rotation) < 0.1 && Sun->GetActorRotation().Pitch == -90) {
			rotation = 0.1;
		}
		
		Sun->AddActorLocalRotation(FRotator(rotation, 0, 0));
		Moon->AddActorLocalRotation(FRotator(rotation, 0, 0));
		SkyLight->GetLightComponent()->RecaptureSky();
		
		CurrentRotation = angle;
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