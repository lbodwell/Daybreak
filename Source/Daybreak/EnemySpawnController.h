// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.h"
#include "EnemySpawnField.h"
#include "DaybreakCharacter.h"
#include "DaybreakEnemyCharacter.h"
#include "EnemySpawnController.generated.h"


UCLASS()
class DAYBREAK_API AEnemySpawnController : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADaybreakEnemyCharacter> Steve;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADaybreakEnemyCharacter> ThiccBoi;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> SpawnFields;
	
	UPROPERTY(EditAnywhere)
    TSoftObjectPtr<ADayNightCycle> DayNightController;
	
	UFUNCTION()
	void OnDayStart(int DayLengthSeconds);

	int enemiesSpawned;
	
	float spawnExponential;
	float spawnFactor;
	float DayLengthSeconds;
	float ThiccBoiChance;
	
	FTimerHandle spawnTimerHandle;
	
	void SpawnTick();

	ADaybreakCharacter* Player;
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
};