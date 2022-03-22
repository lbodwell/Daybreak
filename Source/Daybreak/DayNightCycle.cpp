// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"

// Sets default values
ADayNightCycle::ADayNightCycle(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay(){
	Super::BeginPlay();
	
}

// Called every frame
void ADayNightCycle::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	// Remove
	AdvanceTime(DeltaTime * TurnRate);
	
	if (IsDayTime) {
		// Advance time as monsters spawn.
		// Rewind time when monsters are killed.
		// For testing, check for key press in player controller and listen for event here.
	} else {
		// Advance time when monsters are killed.
		// For testing, check for key press in player controller and listen for event here.
	}
}

void ADayNightCycle::AdvanceTime(float Amount) {
	if (LightSource != NULL && Sun != NULL) {
		FOutputDeviceNull outDevice;
		LightSource->AddActorLocalRotation(FRotator(-Amount, 0, 0));
		Sun->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), outDevice, NULL, true);

		FVector rotation = LightSource->GetActorRotation().Vector();
		IsDayTime = LightSource->GetActorRotation().Pitch <= 0;

		// TODO: Expose a time variable and construct timestamp based on difference between current and prev time through piecewise definition
		
		// Construct polar coords from cartesian. Attempting to map to range of 0-360 degrees. Math needs work.
		float angle = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan(rotation.X * rotation.Z));
		UE_LOG(LogActor, Warning, TEXT("Time: %s"), IsDayTime ? TEXT("Day") : TEXT("Night"));
	}
}

void ADayNightCycle::RewindTime(float Amount) {
	AdvanceTime(-Amount);
}