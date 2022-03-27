// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakUpgradeMenu.h"
#include "DaybreakSword.h"
#include "Kismet/GameplayStatics.h"

bool UDaybreakUpgradeMenu::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;
	
	return true;
}

void UDaybreakUpgradeMenu::NativeConstruct() {
	Super::NativeConstruct();
	
	player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	UpdateUI();
}

FSwordLevel UDaybreakUpgradeMenu::GetCurrentSwordLevel() {
	sword = player->GetSword();
	
	if (sword != nullptr) {
		return sword->CurrentLevel;
	}
	return FSwordLevel();
}

FSwordLevel UDaybreakUpgradeMenu::GetNextSwordLevel() {
	sword = player->GetSword();
	int index = sword->CurrentLevel.Index;
	
	if (sword != nullptr && index < 5) {
		return sword->Levels[index + 1];
	}
	return FSwordLevel();
}

 void UDaybreakUpgradeMenu::UpdateUI_Implementation() {
 } 