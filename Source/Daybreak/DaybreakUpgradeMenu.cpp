// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakUpgradeMenu.h"
#include "DaybreakSword.h"
#include "Kismet/GameplayStatics.h"
#include "Daybreak2DSwordCamera.h"

bool UDaybreakUpgradeMenu::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;
	
	UpgradeProgress = 0;
	isUpgrading = false;
	
	return true;
}

void UDaybreakUpgradeMenu::NativeConstruct() {
	Super::NativeConstruct();
	
	// bind player and upgrade actions
	player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	player->GetPlayerInputComponent()->BindAction("Interact", IE_Pressed, this, &UDaybreakUpgradeMenu::StartUpgrading);
    player->GetPlayerInputComponent()->BindAction("Interact", IE_Released, this, &UDaybreakUpgradeMenu::StopUpgrading);
	
	// get 2DSwordCameras and update their colors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADaybreak2DSwordCamera::StaticClass(), SwordCameras);
	for (int i = 0; i < 2; i++) {
		ADaybreak2DSwordCamera* swordCamera = Cast<ADaybreak2DSwordCamera>(SwordCameras[i]);
		swordCamera->UpdateLevel(swordCamera->ID == 1 ? GetCurrentSwordLevel() : GetNextSwordLevel());
	} 

	UpdateUI();
}

void UDaybreakUpgradeMenu::NativeDestruct() {
	Super::NativeDestruct();
	
	// unbind upgrade actions
	int lastIndex = player->GetPlayerInputComponent()->GetNumActionBindings() - 1;
	player->GetPlayerInputComponent()->RemoveActionBinding(lastIndex);
	player->GetPlayerInputComponent()->RemoveActionBinding(lastIndex - 1);
}

void UDaybreakUpgradeMenu::StartUpgrading() {
	isUpgrading = true;
	GetWorld()->GetTimerManager().SetTimer(upgradeTimerHandle, [&]() {
		if (isUpgrading) {
			UpgradeProgress += 0.01;
			if (UpgradeProgress >= 1) {
				Upgrade();
				UpgradeProgress = 0;
			}
		} else {
			UpgradeProgress -= 0.01;
			if (UpgradeProgress <= 0) {
				GetWorld()->GetTimerManager().ClearTimer(upgradeTimerHandle);
			}
		}
	}, 0.01, true);
}

void UDaybreakUpgradeMenu::StopUpgrading() {
	isUpgrading = false;
}

void UDaybreakUpgradeMenu::Upgrade() {
	player->GetSword()->Upgrade();
	UpdateUI();
	for (int i = 0; i < 2; i++) {
		ADaybreak2DSwordCamera* swordCamera = Cast<ADaybreak2DSwordCamera>(SwordCameras[i]);
		swordCamera->UpdateLevel(swordCamera->ID == 1 ? GetCurrentSwordLevel() : GetNextSwordLevel());
	} 
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
	
	if (sword != nullptr) {
		int index = sword->CurrentLevel.Index;
		
		if (index < 5) {
			return sword->Levels[index + 1];
		}
	}
	return FSwordLevel();
}

 void UDaybreakUpgradeMenu::UpdateUI_Implementation() {
 } 