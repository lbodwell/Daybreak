// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnField.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawnField::AEnemySpawnField()
{
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	RootComponent = SpawnVolume;
}

// Called when the game starts or when spawned
void AEnemySpawnField::BeginPlay()
{
	Super::BeginPlay();
}


FVector AEnemySpawnField::GetRandomPointsInVolume()
{
	FVector Origin = SpawnVolume->Bounds.Origin;
	FVector Extents = SpawnVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extents);
}

