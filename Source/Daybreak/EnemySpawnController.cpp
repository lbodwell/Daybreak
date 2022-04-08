// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawnController.h"
#include <math.h>
#include "Engine.h"
#include "EnemySpawnController.h"
#include "EnemySpawnField.h"
#include "DaybreakCharacter.h"


AEnemySpawnController::AEnemySpawnController() {
	PrimaryActorTick.bCanEverTick = false;
	
	enemyCount = 0;
	Player = nullptr;
	PlayerCamera = nullptr;
	spawnExponential = 2;
}

// Called when the game starts or when spawned
void AEnemySpawnController::BeginPlay() {
	Super::BeginPlay();
	
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PlayerCamera = Player->FindComponentByClass<UCameraComponent>();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnField::StaticClass(), SpawnFields);
	
	//FTimerHandle TimerHandle;
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemySpawnController::SpawnActor, 2.0f, true, 0.5f);
	
	if (DayNightController) {
		// listen to DayNightController for when day starts
		DayNightController->OnDayStart.AddDynamic(this, &AEnemySpawnController::OnDayStart);
	}
}

void AEnemySpawnController::OnDayStart(int dayLengthSeconds) {
	DayLengthSeconds = dayLengthSeconds;
	float enemiesToSpawn = 100;
	spawnFactor = enemiesToSpawn / (float)pow(DayLengthSeconds, spawnExponential);
	
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEnemySpawnController::SpawnTick, 0.2, true);
}

void AEnemySpawnController::SpawnTick() {
	float expectedEnemyCount = spawnFactor * pow(DayLengthSeconds - DayNightController->GetDayLengthSecondsRemaining(), spawnExponential);
	
	while (expectedEnemyCount - (float)enemyCount >= 1) {
		SpawnActor();
		UE_LOG(LogActor, Warning, TEXT("Enemy Count: %d"), enemyCount);
		
		if (enemyCount >= expectedEnemyCount) {
			GetWorldTimerManager().ClearTimer(spawnTimerHandle);
		}
	}
}

void AEnemySpawnController::SpawnActor() {
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
}

AEnemySpawnField* AEnemySpawnController::GetRandomSpawnField() {
	//For Debugging
	int FieldsInView = SpawnFields.Num();

	TArray<AActor*> SpawnFieldsOutOfView = TArray<AActor*>();

	PlayerCamera->GetCameraView(0.5f, CameraInfo);

	FVector NormalDirection = FVector();
	float DotProductAngle = 0.0f;

	for(int i = 0; i < SpawnFields.Num(); i++) {

		NormalDirection = (SpawnFields[i]->GetActorLocation() - PlayerCamera->GetComponentLocation());
		NormalDirection.Normalize();
		DotProductAngle = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Acos(FVector::DotProduct(PlayerCamera->GetForwardVector(), NormalDirection)));

		//if outside of FOV, add to list to choose from
		if (UKismetMathLibrary::Abs(DotProductAngle) > PlayerCamera->FieldOfView / 2) {	
			SpawnFieldsOutOfView.Add(SpawnFields[i]);
			FieldsInView--;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d Fields in View"), FieldsInView);

	if (SpawnFieldsOutOfView.Num() > 0) {

		int index = FMath::RandRange(0, SpawnFieldsOutOfView.Num() - 1);
		return dynamic_cast<AEnemySpawnField*>(SpawnFieldsOutOfView[index]);
	} 
	else {
		if (SpawnFields.Num() > 0) {

			int index = FMath::RandRange(0, SpawnFields.Num() - 1);
			return dynamic_cast<AEnemySpawnField*>(SpawnFields[index]);
		}

		else return NULL;
	}

}