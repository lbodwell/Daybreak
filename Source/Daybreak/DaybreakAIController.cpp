// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADaybreakAIController:: ADaybreakAIController() {
}

void ADaybreakAIController::BeginPlay() {
	Super::BeginPlay();
	
	pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());
	
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<AActor>(player);
	
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADaybreakAIController::ChasePlayer, 0.1, true);
}

void ADaybreakAIController::ChasePlayer() {
	MoveToActor(playerActor, 0.0f, true, true, true, NULL, true);
	float capsuleRadius = 35;
	float distance = (pawn->GetActorLocation() - playerActor->GetActorLocation()).Size() - capsuleRadius * 2;
	if (distance < 25) {
		pawn->Attack();
	}
}