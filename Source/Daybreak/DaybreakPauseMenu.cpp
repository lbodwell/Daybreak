// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakPauseMenu.h"
#include "DaybreakCharacter.h"
#include "Kismet/GameplayStatics.h"


bool UDaybreakPauseMenu::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;
	
	return true;
}

void UDaybreakPauseMenu::NativeConstruct() {
	Super::NativeConstruct();
}

void UDaybreakPauseMenu::NativeDestruct() {
	Super::NativeDestruct();
}

void UDaybreakPauseMenu::ExitMenu() {
	ADaybreakCharacter* player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player) {
		player->Exit();
	}
}