// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnController.h"
#include <math.h>

// Sets default values
AEnemySpawnController::AEnemySpawnController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AEnemySpawnController::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemySpawnController::SpawnActor, 2.0f, true, 0.5f);
}

void AEnemySpawnController::SpawnActor()
{

	//For when spawn fields are implemented
	//FVector Location = getLocationFromSpawnField();
	//FRotator Rotation = getRotationFromSpawnField();

	//For Testing
	FVector Location = FVector(FMath::RandRange(-1900, 1100), FMath::RandRange(-1500, 1500), 600);
	FRotator Rotation = FRotator(0, 0, 0);


	GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotation);
}


