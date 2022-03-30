// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"

// Sets default values
ADayNightCycle::ADayNightCycle() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CurrentRotation = 180;
	IsDayTime = true;
	TestAlternator = 0;
}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay() {
	Super::BeginPlay();
	
	SetTime(180);
	UpdateRotation();

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
void ADayNightCycle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ADayNightCycle::UpdateRotation() {
	if (Sun != nullptr) {
		// Extract direction unit vector from sun directional light's rotation.
		FVector rotation = Sun->GetActorRotation().Vector();
		// Convert cartesian coordinates to parametric and adjust range to 0-360 degrees.
		CurrentRotation = (180 - (UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(rotation.Z, rotation.Y))));
		// If rotation is between 180 and 360 degrees, it's day time.
		IsDayTime = CurrentRotation >= 180;

		UE_LOG(LogActor, Warning, TEXT("Time: %f (%s)"), GetTimeInHours(), IsDayTime ? TEXT("day") : TEXT("night"));
	}
}

bool ADayNightCycle::AdvanceTime(float Angle) {
	// Advancing is only allowed by positive angles
	if (Sun != nullptr && Angle >= 0) {
		// Apply local rotation to sun's pitch.
		Sun->AddActorLocalRotation(FRotator(Angle, 0, 0));
		
		if (Moon != nullptr) {
			// Apply same local rotation to moon's pitch if it exists.
			Moon->AddActorLocalRotation(FRotator(Angle, 0, 0));
		}

		// Update rotation of internal clock.
		UpdateRotation();

		return true;
	}

	return false;
}

bool ADayNightCycle::RewindTime(float Angle) {
	// Rewinding is only allowed during the day by negative angles (inputted as absolute value)
	if (Sun != nullptr && IsDayTime && Angle >= 0) {
		if (UKismetMathLibrary::Round(CurrentRotation - Angle) > 180) {
			// If specified angle would not go back to previous night, apply local rotation to sun's pitch at negative angle.
			Sun->AddActorLocalRotation(FRotator(-Angle, 0, 0));

			if (Moon != nullptr) {
				// Apply same local rotation to moon's pitch if it exists.
				Moon->AddActorLocalRotation(FRotator(-Angle, 0, 0));
			}
		} else {
			// Otherwise, apply local rotation of opposite of the difference between current rotation and 180 degrees (start of day) to sun's pitch.
			Sun->AddActorLocalRotation(FRotator(-(CurrentRotation - 180), 0, 0));

			if (Moon != nullptr) {
				// Apply same local rotation to moon's pitch if it exists.
				Moon->AddActorLocalRotation(FRotator(-(CurrentRotation - 180), 0, 0));
			}
		}

		// Update rotation of internal clock.
		UpdateRotation();

		return true;
	}

	return false;
}

bool ADayNightCycle::SetTime(float Angle) {
	if (Sun != nullptr && Angle >= 0 && Angle <= 360) {
		// Apply local rotation of difference between target angle and current rotation to sun's pitch.
		Sun->AddActorLocalRotation(FRotator(Angle - CurrentRotation, 0, 0));

		if (Moon != nullptr) {
			// Apply same local rotation to moon's pitch if it exists.
			Moon->AddActorLocalRotation(FRotator(Angle - CurrentRotation, 0, 0));
		}

		// Update rotation of internal clock.
		UpdateRotation();

		return true;
	}

	return false;
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