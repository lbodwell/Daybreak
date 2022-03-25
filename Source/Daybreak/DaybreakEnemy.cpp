// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakEnemy.h"

// Sets default values
ADaybreakEnemy::ADaybreakEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADaybreakEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADaybreakEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADaybreakEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

