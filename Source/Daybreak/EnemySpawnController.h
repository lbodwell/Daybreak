// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.h"
#include "EnemySpawnField.h"
#include "DaybreakEnemyCharacter.h"
#include "EnemySpawnController.generated.h"


UCLASS()
class DAYBREAK_API AEnemySpawnController : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADaybreakEnemyCharacter> EnemyToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> SpawnFields;

	FTimerHandle TimerHandle;

	ACharacter* Player;
	UCameraComponent* PlayerCamera;
	FMinimalViewInfo CameraInfo;

	AEnemySpawnField* GetRandomSpawnField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AEnemySpawnController();

	void SpawnActor();
	int enemyCount;


};
