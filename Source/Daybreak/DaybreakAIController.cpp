// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakAIController.h"
#include "EnemyStates.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Navigation/CrowdFollowingComponent.h"

ADaybreakAIController:: ADaybreakAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

}

void ADaybreakAIController::BeginPlay() {
	Super::BeginPlay();

	CurrentState = new Idle;
	
	pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());
	
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<AActor>(player);

	TArray<AActor*> PortalMeshes;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Portal"), PortalMeshes);
	PortalLocation = PortalMeshes[0]->GetActorLocation();

	TArray<AActor*> DayNightCycles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightCycle::StaticClass(), DayNightCycles);
	DayNightCycle = dynamic_cast<ADayNightCycle*>(DayNightCycles[0]);

	if (DayNightCycle) {
		// listen to DayNightController for when day/night starts
		DayNightCycle->OnDayStart.AddDynamic(this, &ADaybreakAIController::OnDayStart);
		DayNightCycle->OnNightStart.AddDynamic(this, &ADaybreakAIController::OnNightStart);
	}
	
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


//Listener Functions
void ADaybreakAIController::OnDayStart(int DayLengthSeconds) {
	UE_LOG(LogTemp, Warning, TEXT("OnDayStart"));
	SetState(new Daytime);
}

void ADaybreakAIController::OnNightStart() {
	UE_LOG(LogTemp, Warning, TEXT("OnNightStart"));
	SetState(new Nighttime);
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

bool ADaybreakAIController::GetIsDay() {
	return DayNightCycle->GetIsDay();
}