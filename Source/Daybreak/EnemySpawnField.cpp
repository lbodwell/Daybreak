// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnField.h"

// Sets default values
AEnemySpawnField::AEnemySpawnField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FieldWidth = 500;
	FieldHeight = 500;
	FieldDepth = 500;

}

// Called when the game starts or when spawned
void AEnemySpawnField::BeginPlay()
{
	Super::BeginPlay();
	
}

tuple<FVector, FVector> AEnemySpawnField::getSpawnFieldBoundaries()
{
	FVector Location = this->GetActorLocation();
	int XOffset = FieldWidth / 2;
	int YOffset = FieldHeight / 2;
	int ZOffset = FieldDepth / 2;

	FVector corner1 = FVector(Location.X - XOffset, Location.Y - YOffset, Location.Z - ZOffset);
	FVector corner2 = FVector(Location.X + XOffset, Location.Y + YOffset, Location.Z + ZOffset);

	return tuple<FVector, FVector>(corner1, corner2);
}
