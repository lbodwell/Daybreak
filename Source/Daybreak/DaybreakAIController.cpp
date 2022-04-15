// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakAIController.h"
#include "EnemyStates.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADaybreakAIController:: ADaybreakAIController() {
}

void ADaybreakAIController::BeginPlay() {
	Super::BeginPlay();

	CurrentState = new Idle;
	
	pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());
	
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<AActor>(player);

	PortalLocation = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Portal"))[0]->GetOwner()->GetActorLocation();
	
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADaybreakAIController::RunState, 0.1, true);
}

void ADaybreakAIController::SetState(EnemyState* newState) {
	CurrentState->exit(this);
	CurrentState = newState;
	CurrentState->enter(this);
}

void ADaybreakAIController::RunState() {
	CurrentState->run(this);
}

void ADaybreakAIController::ChasePlayer() {
	CheckPawns();

	if (pawn->IsAlive) {
		MoveToActor(playerActor, 0.0f, true, true, true, NULL, true);
		float capsuleRadius = 35;
		float distance = (pawn->GetActorLocation() - playerActor->GetActorLocation()).Size() - capsuleRadius * 2;
	}
}

void ADaybreakAIController::Attack() {
	pawn->Attack();
}


//Helper Functions
float ADaybreakAIController::GetDistanceToPlayer() {
	CheckPawns();

	return (pawn->GetActorLocation() - playerActor->GetActorLocation()).Size() - 70;
}

float ADaybreakAIController::GetDistanceToPortal() {
	CheckPawns();

	return (pawn->GetActorLocation() - PortalLocation).Size() - 70;
}

FVector ADaybreakAIController::GetRandomNearbyLocation() {
	CheckPawns();
	pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());
	
	return pawn->GetActorLocation() + FVector(FMath::RandRange(-2000, 2000), FMath::RandRange(-2000, 2000), 0);
}

void ADaybreakAIController::CheckPawns() {
	if (pawn == nullptr || playerActor == nullptr) {
		pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());

		ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		playerActor = Cast<AActor>(player);
	}
}

FVector ADaybreakAIController::GetPortalLocation() {
	return PortalLocation;
}