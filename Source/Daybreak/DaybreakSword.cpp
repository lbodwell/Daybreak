// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakSword.h"
#include "DaybreakEnemyCharacter.h"
#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"
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

	// Initialize audio components
	static ConstructorHelpers::FObjectFinder<USoundCue> attackImpactCueObj(TEXT("SoundCue'/Game/Audio/Player/Attack/Player_Attack_Impact_Cue.Player_Attack_Impact_Cue'"));
	if (attackImpactCueObj.Succeeded()) {
		AttackImpactCue = attackImpactCueObj.Object;
		attackImpactSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackImpactSound"));
		attackImpactSound->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> darkstoneCollectCueObj(TEXT("SoundCue'/Game/Audio/Player/Materials/Player_Darkstone_Collect_Cue.Player_Darkstone_Collect_Cue'"));
	if (darkstoneCollectCueObj.Succeeded()) {
		DarkstoneCollectCue = darkstoneCollectCueObj.Object;
		darkstoneCollectSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DarkstoneCollectSound"));
		darkstoneCollectSound->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> weaponUpgradeCueObj(TEXT("SoundCue'/Game/Audio/Player/UI/Anvil/UI_Anvil_Weapon_Upgrade_Cue.UI_Anvil_Weapon_Upgrade_Cue'"));
	if (weaponUpgradeCueObj.Succeeded()) {
		WeaponUpgradeCue = weaponUpgradeCueObj.Object;
		weaponUpgradeSound = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponUpgradeSound"));
		weaponUpgradeSound->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ADaybreakSword::BeginPlay() {
	Super::BeginPlay();

	// Set audio component sound cues
	if (attackImpactSound) {
		attackImpactSound->SetSound(AttackImpactCue);
	}
	if (darkstoneCollectSound) {
		darkstoneCollectSound->SetSound(DarkstoneCollectCue);
	}
	if (weaponUpgradeSound) {
		weaponUpgradeSound->SetSound(WeaponUpgradeCue);
	}
}

void ADaybreakSword::Upgrade() {
	if (CurrentLevel.Index < 5) {		
		CurrentLevel = Levels[CurrentLevel.Index + 1];
		LastCost = CurrentLevel.Cost;
		
		if (CurrentLevel.Index < 5) {
			Cost = Levels[CurrentLevel.Index + 1].Cost;
		}
		
		UpdateEffect();

		if (weaponUpgradeSound) {
			weaponUpgradeSound->Play(0);
		}

		// Upgrade sound effect here
	}
}

void ADaybreakSword::Attack(class AActor* overlappedActor, class AActor* otherActor) {
	if (otherActor != nullptr && otherActor != this) {
		ADaybreakEnemyCharacter* enemy = Cast<ADaybreakEnemyCharacter>(otherActor);
		ADestructibleResource* resource = Cast<ADestructibleResource>(otherActor);

		// if sword hits an enemy
		if (enemy != nullptr && Hitting) {
			enemy->ReceiveDamage(10 + CurrentLevel.Damage * 10);

			if (attackImpactSound && !attackImpactSound->IsPlaying()) {
				attackImpactSound->Play(0);
			}
		}

		// if sword hits a resource
		if (resource != nullptr && Hitting) {
			UE_LOG(LogActor, Warning, TEXT("RESOURCE"));
			if (darkstoneCollectSound) {
				darkstoneCollectSound->Play(0);
			}
			resource->Damage(2 + CurrentLevel.Damage * 10);
		}
	}
}

void ADaybreakSword::UpdateEffect_Implementation() {
}
