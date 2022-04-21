// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "DaybreakKillVolume.generated.h"

/**
 * 
 */
UCLASS()
class DAYBREAK_API ADaybreakKillVolume : public AVolume {
	GENERATED_BODY()

	ADaybreakKillVolume();
	
private:
	void ActorEnteredVolume(AActor* thisActor, AActor* otherActor);
};
