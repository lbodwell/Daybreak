// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakPauseMenu.h"

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