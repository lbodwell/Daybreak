// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakAIController.h"
#include "EnemyStates.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Navigation/CrowdFollowingComponent.h"

ADaybreakAIController::ADaybreakAIController(const FObjectInitializer& ObjectInitializer)
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
	
	PortalController = PortalMeshes[0]->FindComponentByClass<UPortalController>();

	TArray<AActor*> DayNightCycles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightCycle::StaticClass(), DayNightCycles);
	DayNightCycle = dynamic_cast<ADayNightCycle*>(DayNightCycles[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APathfindingWaypoint::StaticClass(), Waypoints);
	WaypointVisited = false;

	//attach listeners
	if (DayNightCycle) {
		DayNightCycle->OnDayStart.AddDynamic(this, &ADaybreakAIController::OnDayStart);
		DayNightCycle->OnNightStart.AddDynamic(this, &ADaybreakAIController::OnNightStart);
	}
	if (PortalController) {
		PortalController->OnPortalActivate.AddDynamic(this, &ADaybreakAIController::OnPortalActivate);
		PortalController->OnPortalDeactivate.AddDynamic(this, &ADaybreakAIController::OnPortalDeactivate);
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
	//UE_LOG(LogTemp, Warning, TEXT("OnDayStart"));
	SetState(new Daytime);
}

void ADaybreakAIController::OnNightStart() {
	//UE_LOG(LogTemp, Warning, TEXT("OnNightStart"));
	SetState(new Nighttime);
}

void ADaybreakAIController::OnPortalActivate() {
	//UE_LOG(LogTemp, Warning, TEXT("OnPortalActivate"));
	SetState(new VisitWaypoint);
}

void ADaybreakAIController::OnPortalDeactivate() {
	//UE_LOG(LogTemp, Warning, TEXT("OnPortalDeactivate"));
	SetState(new ChasePlayerNight);
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

FVector ADaybreakAIController::GetRandomWaypoint() {
	if (Waypoints.Num() == 0) { 
		SetState(new SwarmPortal); 
		return PortalLocation;
	}

	int index = FMath::RandRange(0, Waypoints.Num() - 1);

	return Waypoints[index]->GetActorLocation();
}

void ADaybreakAIController::CheckPawns() {
	if (pawn == nullptr || playerActor == nullptr) {
		if (GetPawn()) {
			pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());
		}

		ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		playerActor = Cast<AActor>(player);
	}
}

float ADaybreakAIController::GetCapsuleRadius() {
	CheckPawns();
	pawn = Cast<ADaybreakEnemyCharacter>(GetPawn());
	
	return pawn->CapsuleRadius;
}

FVector ADaybreakAIController::GetPortalLocation() {
	return PortalLocation;
}

bool ADaybreakAIController::GetIsDay() const {
	return DayNightCycle->GetIsDay();
}

bool ADaybreakAIController::GetWaypointIsVisited() const {
	return WaypointVisited;
}

void ADaybreakAIController::SetWaypointVisited(bool newIsVisited) {
	WaypointVisited = newIsVisited;
}