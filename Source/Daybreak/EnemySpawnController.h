// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.h"
#include "EnemySpawnController.generated.h"


UCLASS()
class DAYBREAK_API AEnemySpawnController : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> ActorToSpawn;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	//std::vector<ASpawnField> SpawnFields;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ADayNightCycle* DayNightCycle;

	FTimerHandle TimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AEnemySpawnController();

	void SpawnActor();
	int enemyCount;


};
