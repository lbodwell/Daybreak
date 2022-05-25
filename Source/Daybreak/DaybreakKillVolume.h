// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DaybreakKillVolume.generated.h"

/**
 * 
 */
UCLASS()
class DAYBREAK_API ADaybreakKillVolume : public AActor {
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void KillPlayer(AActor* thisActor, AActor* otherActor);

	virtual void BeginPlay() override;
};
