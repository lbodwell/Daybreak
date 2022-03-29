// Fill out your copyright notice in the Description page of Project Settings.


#include "Daybreak2DSwordCamera.h"
#include "DaybreakSword.h"

// Sets default values
ADaybreak2DSwordCamera::ADaybreak2DSwordCamera() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADaybreak2DSwordCamera::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ADaybreak2DSwordCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ADaybreak2DSwordCamera::UpdateLevel_Implementation(FSwordLevel level) {
} 
