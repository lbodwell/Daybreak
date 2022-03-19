// Copyright Epic Games, Inc. All Rights Reserved.

#include "DaybreakGameMode.h"
#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADaybreakGameMode::ADaybreakGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Sword_Base_Character"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
