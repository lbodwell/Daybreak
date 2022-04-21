// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakKillVolume.h"
#include "DaybreakCharacter.h"

ADaybreakKillVolume::ADaybreakKillVolume() {
	OnActorBeginOverlap.AddDynamic(this, &ADaybreakKillVolume::ActorEnteredVolume);
}

void ADaybreakKillVolume::ActorEnteredVolume(AActor* thisActor, AActor* otherActor) {
	UE_LOG(LogActor, Warning, TEXT("ACTOR!"));
	ADaybreakCharacter* player = Cast<ADaybreakCharacter>(otherActor);

	if (player) {
		player->Destroy();
	}
}