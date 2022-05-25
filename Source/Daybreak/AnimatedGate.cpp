// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedGate.h"
#include <Engine.h>

// Sets default values
AAnimatedGate::AAnimatedGate() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Direction = 0;
}

void AAnimatedGate::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> DayNightCycles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightCycle::StaticClass(), DayNightCycles);
	DayNightCycle = dynamic_cast<ADayNightCycle*>(DayNightCycles[0]);
}

// Called every frame
void AAnimatedGate::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	FVector Location = GetActorLocation();

	// Go up if it's day & low
	if (DayNightCycle->GetIsDay()) {

		if (GetActorLocation().Z <= MaxHeight) {
			Direction = 1;
		}
		else Direction = 0;
	}

	// Go down if it's night & high
	if (!DayNightCycle->GetIsDay()) {

		if (GetActorLocation().Z >= MinHeight) {
			Direction = -1;
		}
		else Direction = 0;
	}

	Location += GetActorUpVector() * Speed * DeltaTime * Direction;
	SetActorLocation(Location);
}

