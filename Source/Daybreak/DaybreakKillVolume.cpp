// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakKillVolume.h"
#include "DaybreakCharacter.h"

void ADaybreakKillVolume::BeginPlay() {
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ADaybreakKillVolume::KillPlayer);
}

void ADaybreakKillVolume::KillPlayer(AActor* thisActor, AActor* otherActor) {
	ADaybreakCharacter* player = Cast<ADaybreakCharacter>(otherActor);

	if (player) {
		player->Destroy();
	}
}