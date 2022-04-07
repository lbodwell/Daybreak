// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakAIController.h"
#include "EnemyStates.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADaybreakAIController:: ADaybreakAIController() {
}

void ADaybreakAIController::BeginPlay() {
	Super::BeginPlay();

	CurrentState = &Idle::getInstance();
	
	pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());
	
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<AActor>(player);
	
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADaybreakAIController::RunState, 0.1, true);
}

void ADaybreakAIController::SetState(EnemyState& newState) {
	CurrentState->exit(this);
	CurrentState = &newState;
	CurrentState->enter(this);
}

void ADaybreakAIController::RunState() {
	CurrentState->run(this);
}

void ADaybreakAIController::ChasePlayer() {
	if (pawn != nullptr && pawn->IsAlive && playerActor != nullptr) {
		MoveToActor(playerActor, 0.0f, true, true, true, NULL, true);
		float capsuleRadius = 35;
		float distance = (pawn->GetActorLocation() - playerActor->GetActorLocation()).Size() - capsuleRadius * 2;
		if (distance < 40) {
		  pawn->Attack();
		}
	}
	else {
		pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());

		ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		playerActor = Cast<AActor>(player);
	}
}


//Helper Functions
float ADaybreakAIController::GetDistanceToPlayer() const {
	return (pawn->GetActorLocation() - playerActor->GetActorLocation()).Size();
}

FVector ADaybreakAIController::GetRandomNearbyLocation() const {
	return pawn->GetActorLocation() + FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), FMath::RandRange(-500, 500));
}