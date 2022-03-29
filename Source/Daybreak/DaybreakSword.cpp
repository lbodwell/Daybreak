// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakSword.h"

// Sets default values
ADaybreakSword::ADaybreakSword() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Levels.Emplace(); // default
	Levels.Emplace(1, "Darkened Steel Sword", FLinearColor(0.05, 0.1, 1, 1), 0.1, 0.1, 0.1, 400);
	Levels.Emplace(2, "Draconic Steel Sword", FLinearColor(0, 0.6, 0.15, 1), 0.3, 0.2, 0.2, 800);
	Levels.Emplace(3, "Darkonium Sword", FLinearColor(0.25, 0.03, 1, 1), 0.6, 0.4, 0.5, 1400);
	Levels.Emplace(4, "Emblazened Darkonium Sword", FLinearColor(1, 0.2, 0.1, 1), 0.75, 0.8, 0.75, 2000);
	Levels.Emplace(5, "The Sword of Darkosius", FLinearColor(1, 1, 1, 1), 1, 1, 1, 2400);
	
	CurrentLevel = Levels[0];
}

// Called when the game starts or when spawned
void ADaybreakSword::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ADaybreakSword::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ADaybreakSword::Upgrade() {
	if (CurrentLevel.Index < 5) {
		CurrentLevel = Levels[CurrentLevel.Index + 1];
		UpdateEffect();
	}
}

void ADaybreakSword::UpdateEffect_Implementation() {
 } 
