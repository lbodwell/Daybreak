// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"

// Sets default values
ADayNightCycle::ADayNightCycle() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	IsDayTime = true;
	CurrentRotation = 180;
	TestAlternator = 0;
}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay() {
	Super::BeginPlay();

	UE_LOG(LogActor, Warning, TEXT("Time: %f (%s)"), GetTimeInHours(), IsDayTime ? TEXT("day") : TEXT("night"));

	// Advances time by 1 hour every second at night. Alternates between advancing time by 2 hours and rewinding time by 1 hour every second during the day.
	// Uncomment for testing time manipulation.
	/*FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [&]() {
		if (IsDayTime) {
			if (TestAlternator == 0) {
				UE_LOG(LogActor, Warning, TEXT("Advancing..."));
				AdvanceTime(30);
				TestAlternator = 1;
			} else {
				UE_LOG(LogActor, Warning, TEXT("Rewinding..."));
				RewindTime(15);
				TestAlternator = 0;
			}
		} else {
			UE_LOG(LogActor, Warning, TEXT("Advancing..."));
			AdvanceTime(15);
		}
	}, 1, true);*/
}

// Called every frame
void ADayNightCycle::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ADayNightCycle::AdvanceTime(float Angle) {
	if (LightSource != NULL && Sun != NULL) {
		// Used for calling blueprint functions.
		FOutputDeviceNull outDevice;
		// Apply local rotation to light source pitch. Angle needs to be negative to go in the right direction.
		LightSource->AddActorLocalRotation(FRotator(-Angle, 0, 0));
		// Use blueprint function to update sun position based on light source rotation.
		Sun->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), outDevice, NULL, true);

		// Extract direction unit vector from light source rotation
		FVector rotation = LightSource->GetActorRotation().Vector();
		// Convert cartesian coordinates to parametric and adjust range to 0-360 degrees.
		CurrentRotation = (180 - (UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(rotation.Z, rotation.X))));
		// If rotation is between 180 and 360 degrees, it's day time.
		IsDayTime = CurrentRotation >= 180;

		UE_LOG(LogActor, Warning, TEXT("Time: %f (%s)"), GetTimeInHours(), IsDayTime ? TEXT("day") : TEXT("night"));
	}
}

void ADayNightCycle::RewindTime(float Angle) {
	// Only allow rewinding time during the day
	if (IsDayTime) {
		if (UKismetMathLibrary::Round(CurrentRotation - Angle) > 180) {
			// If specified angle would not go back to previous night, rewind time by that angle.
			AdvanceTime(-Angle);
		} else {
			// Otherwise, rewind time by the difference between current rotation and 180 degrees (start of day).
			AdvanceTime(-(CurrentRotation - 180));
		}
	}	
}

float ADayNightCycle::GetTimeInHours() {
	// 15 degrees = 1 hour. Start of day should correspond to 180 degrees and 6AM.
	float scaledTime = (CurrentRotation / 15.0f) - 6;

	if (scaledTime > 0) {
		return scaledTime;
	} else {
		// Normalizing time by adding 6 makes it so last 6 hours overflow to negative. Fix this by adding 24.
		return 24 + scaledTime;
	}
}