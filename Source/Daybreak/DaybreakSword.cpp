// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakSword.h"
#include "DaybreakEnemyCharacter.h"
#include "DaybreakCharacter.h"
#include <Engine.h>

// Sets default values
ADaybreakSword::ADaybreakSword() : IDaybreakEquipment() {	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Levels.Emplace(); // default
	Levels.Emplace(1, "Darkened Steel Sword", FLinearColor(0, 0.5, 0.1, 1), 0.1, 0.1, "Fire", 400);
	Levels.Emplace(2, "Draconic Steel Sword", FLinearColor(0, 0.05, 0.75, 1), 0.3, 0.2, "Fire", 800);
	Levels.Emplace(3, "Darkonium Sword", FLinearColor(0.25, 0.05, 1, 1), 0.6, 0.4, "Fire", 1400);
	Levels.Emplace(4, "Emblazened Darkonium Sword", FLinearColor(1, 0.25, 0.05, 1), 0.75, 0.8, "Fire", 2000);
	Levels.Emplace(5, "The Sword of Darkosius", FLinearColor(0.55, 0.55, 0, 1), 1, 1, "Fire", 2400);
	
	CurrentLevel = Levels[0];
	LastCost = CurrentLevel.Cost;
	Cost = Levels[1].Cost;

	OnActorBeginOverlap.AddDynamic(this, &ADaybreakSword::Attack);
	Hitting = false;
}

// Called when the game starts or when spawned
void ADaybreakSword::BeginPlay() {
	Super::BeginPlay();
}

void ADaybreakSword::Upgrade() {
	if (CurrentLevel.Index < 5) {		
		CurrentLevel = Levels[CurrentLevel.Index + 1];
		LastCost = CurrentLevel.Cost;
		
		if (CurrentLevel.Index < 5) {
			Cost = Levels[CurrentLevel.Index + 1].Cost;
		}
		
		UpdateEffect();
	}
}

void ADaybreakSword::Attack(class AActor* overlappedActor, class AActor* otherActor) {
	if (otherActor != nullptr && otherActor != this) {
		ADaybreakEnemyCharacter* enemy = Cast<ADaybreakEnemyCharacter>(otherActor);
		ADestructibleResource* resource = Cast<ADestructibleResource>(otherActor);

		// if sword hits an enemy
		if (enemy != nullptr && Hitting) {
			enemy->ReceiveDamage(10 + CurrentLevel.Damage * 10);
		}

		// if sword hits a resource
		if (resource != nullptr && Hitting) {
			resource->Destroy();
		}
	}
}

void ADaybreakSword::UpdateEffect_Implementation() {
}
