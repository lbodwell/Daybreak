// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakArmor.h"
#include "DaybreakEnemyCharacter.h"
#include "DaybreakCharacter.h"
#include <Engine.h>

// Sets default values
ADaybreakArmor::ADaybreakArmor() : IDaybreakEquipment() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Levels.Emplace(); // default
	Levels.Emplace(1, "Darkened Steel Armor", FLinearColor(0, 0.5, 0.1, 1), 0.2, 0.2, 0.2, 0.2, 600);
	Levels.Emplace(2, "Draconic Steel Armor", FLinearColor(0, 0.05, 0.75, 1), 0.4, 0.4, 0.4, 0.4, 1200);
	Levels.Emplace(3, "Darkonium Armor", FLinearColor(0.25, 0.05, 1, 1), 0.6, 0.6, 0.6, 0.6, 1700);
	Levels.Emplace(4, "Emblazened Darkonium Armor", FLinearColor(1, 0.25, 0.05, 1), 0.8, 0.8, 0.8, 0.8, 2200);
	Levels.Emplace(5, "The Armor of Darkosius", FLinearColor(0.55, 0.55, 0, 1), 1, 1, 1, 1, 2800);
	
	CurrentLevel = Levels[0];
	LastCost = CurrentLevel.Cost;
	Cost = Levels[1].Cost;
}

// Called when the game starts or when spawned
void ADaybreakArmor::BeginPlay() {
	Super::BeginPlay();
}

void ADaybreakArmor::Upgrade() {
	if (CurrentLevel.Index < 5) {
		CurrentLevel = Levels[CurrentLevel.Index + 1];
		LastCost = CurrentLevel.Cost;
		
		if (CurrentLevel.Index < 5) {
			Cost = Levels[CurrentLevel.Index + 1].Cost;
		}
		
		ADaybreakCharacter* player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		player->UpdateHealth();
	}
}
