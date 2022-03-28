// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawnController.h"
#include <math.h>
#include "Engine.h"
#include "EnemySpawnController.h"
#include "EnemySpawnField.h"


// Sets default values
AEnemySpawnController::AEnemySpawnController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	enemyCount = 0;

}

// Called when the game starts or when spawned
void AEnemySpawnController::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnField::StaticClass(), SpawnFields);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemySpawnController::SpawnActor, 2.0f, true, 0.5f);
}

void AEnemySpawnController::SpawnActor()
{

	AEnemySpawnField* SpawnField = GetRandomSpawnField();
	FVector Location;
	FRotator Rotation = FRotator(0, 0, 0);

	if (SpawnField != NULL) {
		Location = SpawnField->GetRandomPointsInVolume();
	}
	else {
		Location = FVector(0, 0, 600);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Field Not Found"));
	}
	


	GetWorld()->SpawnActor<APawn>(EnemyToSpawn, Location, Rotation);
	enemyCount++;
	DayNightCycle->AdvanceTime(15);
}

AEnemySpawnField* AEnemySpawnController::GetRandomSpawnField()
{
	if (SpawnFields.Num() > 0) {
		int index = FMath::RandRange(0, SpawnFields.Num() - 1);

		return dynamic_cast<AEnemySpawnField*>(SpawnFields[index]);
	}
	else return NULL;
}


