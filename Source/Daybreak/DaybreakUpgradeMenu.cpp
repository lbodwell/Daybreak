// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakUpgradeMenu.h"
#include "DaybreakSword.h"
#include "DaybreakArmor.h"
#include "Kismet/GameplayStatics.h"
#include "DaybreakEquipment.h"

bool UDaybreakUpgradeMenu::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;
	
	return true;
}

void UDaybreakUpgradeMenu::NativeConstruct() {
	Super::NativeConstruct();
	
	player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	sword = player->GetSword();
	armor = player->GetArmor();
	
	player->GetPlayerInputComponent()->BindAction<FOnUpgrade>("Interact", IE_Pressed, this, &UDaybreakUpgradeMenu::StartUpgrading, Cast<IDaybreakEquipment>(sword));
    player->GetPlayerInputComponent()->BindAction<FOnUpgrade>("Interact", IE_Released, this, &UDaybreakUpgradeMenu::StopUpgrading, Cast<IDaybreakEquipment>(sword));
	
	player->GetPlayerInputComponent()->BindAction<FOnUpgrade>("Interact2", IE_Pressed, this, &UDaybreakUpgradeMenu::StartUpgrading, Cast<IDaybreakEquipment>(armor));
    player->GetPlayerInputComponent()->BindAction<FOnUpgrade>("Interact2", IE_Released, this, &UDaybreakUpgradeMenu::StopUpgrading, Cast<IDaybreakEquipment>(armor));
	
	// create timer to upgrade equipment
	GetWorld()->GetTimerManager().SetTimer(upgradeTimerHandle, [&]() {
		for (int i = 0; i < equipmentBeingUpgraded.Num(); i++) {
			IDaybreakEquipment* equipment = equipmentBeingUpgraded[i];
			
			if (equipment->GetIsUpgrading() && GetDarkStoneAvailable() >= equipment->Cost) {
				if (equipment->IncreaseUpgradeProgress(0.01)) {
					player->DarkStone -= equipment->LastCost;
					UpdateUI();
				}
			} else {
				if (equipment->DecreaseUpgradeProgress(0.01)) {
					equipmentBeingUpgraded.Remove(equipment);
				}
			}
			UpdateUI();
		}
	}, 0.01, true);
	
	UpdateUI();
}

void UDaybreakUpgradeMenu::NativeDestruct() {
	Super::NativeDestruct();
	
	// unbind upgrade actions
	int lastIndex = player->GetPlayerInputComponent()->GetNumActionBindings() - 1;
	for (int i = 0; i < 4; i++) {
		player->GetPlayerInputComponent()->RemoveActionBinding(lastIndex - i);
	}
	
	GetWorld()->GetTimerManager().ClearTimer(upgradeTimerHandle);
	
	// if there was equipment being upgraded when the menu is closed, reset their upgrade progress
	for (int i = 0; i < equipmentBeingUpgraded.Num(); i++) {
		equipmentBeingUpgraded[i]->ResetUpgradeProgress();
	}
}

void UDaybreakUpgradeMenu::StartUpgrading(IDaybreakEquipment* equipment) {
	if (equipment && GetDarkStoneAvailable() >= equipment->Cost) {
		equipment->SetIsUpgrading(true);
		if (!equipmentBeingUpgraded.Contains(equipment)) {
			equipmentBeingUpgraded.Add(equipment);
		}
	}
}

void UDaybreakUpgradeMenu::StopUpgrading(IDaybreakEquipment* equipment) {
	if (equipment) {
		equipment->SetIsUpgrading(false);
	}
}

FSwordLevel UDaybreakUpgradeMenu::GetCurrentSwordLevel() {
	if (sword) {
		return sword->CurrentLevel;
	}
	return FSwordLevel();
}

FSwordLevel UDaybreakUpgradeMenu::GetNextSwordLevel() {
	if (sword && sword->CurrentLevel.Index < 5) {
		return sword->Levels[sword->CurrentLevel.Index + 1];
	}
	return FSwordLevel();
}

FArmorLevel UDaybreakUpgradeMenu::GetCurrentArmorLevel() {
	if (armor) {
		return armor->CurrentLevel;
	}
	return FArmorLevel();
}

FArmorLevel UDaybreakUpgradeMenu::GetNextArmorLevel() {
	if (armor && armor->CurrentLevel.Index < 5) {
		return armor->Levels[armor->CurrentLevel.Index + 1];
	}
	return FArmorLevel();
}

float UDaybreakUpgradeMenu::GetSwordUpgradeProgress() {
	return sword ? sword->GetUpgradeProgress() : 0;
}

float UDaybreakUpgradeMenu::GetArmorUpgradeProgress() {
	return armor ? armor->GetUpgradeProgress() : 0;
}

float UDaybreakUpgradeMenu::GetDarkStoneAvailable() {
	return player->DarkStone;
}

 void UDaybreakUpgradeMenu::UpdateUI_Implementation() {
 } 