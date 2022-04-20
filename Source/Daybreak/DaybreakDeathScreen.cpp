// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakDeathScreen.h"
#include "DaybreakCharacter.h"
#include "Kismet/GameplayStatics.h"


bool UDaybreakDeathScreen::Initialize() {
	const bool success = Super::Initialize();
	if (!success) return false;

	return true;
}

void UDaybreakDeathScreen::NativeConstruct() {
	Super::NativeConstruct();
}

void UDaybreakDeathScreen::NativeDestruct() {
	Super::NativeDestruct();
}

void UDaybreakDeathScreen::ExitMenu() {
	ADaybreakCharacter* player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player) {
		player->Exit();
	}
}

