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
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADaybreakAIController::ChasePlayer, 0.25, true);
}

void ADaybreakAIController::ChasePlayer() {
	if (!pawn->GetAttacking()) {
		EPathFollowingRequestResult::Type result = MoveToActor(playerActor, 10.0f, true, true, true, NULL, true);
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal) {
			pawn->Attack();
		}
	}
}