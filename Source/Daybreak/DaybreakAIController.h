// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DaybreakEnemyCharacter.h"
#include "EnemyState.h"
#include "DayNightCycle.h"
#include "PathfindingWaypoint.h"
#include "PortalController.h"
#include "DaybreakAIController.generated.h"

//forward declaration to resolve circular dependency
class EnemyState;

/**
 * 
 */
UCLASS()
class DAYBREAK_API ADaybreakAIController : public AAIController {
	GENERATED_BODY()
	
	public:
		ADaybreakAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
		
		virtual void BeginPlay() override;

		inline EnemyState* getCurrentState() const { return CurrentState; }
		float GetDistanceToPlayer();
		float GetDistanceToPortal();
		FVector GetPortalLocation();
		FVector GetRandomNearbyLocation();
		FVector GetRandomWaypoint();
		void SetWaypointVisited(bool isVisited);
		bool GetIsDay() const;
		bool GetWaypointIsVisited() const;

		void SetState(EnemyState* newState);

		void ChasePlayer();
		void Attack();
		
		float GetCapsuleRadius();
		
	protected:
		ADaybreakEnemyCharacter* pawn;
		AActor* playerActor;
		FVector PortalLocation;

		EnemyState* CurrentState;
		ADayNightCycle* DayNightCycle;
		UPortalController* PortalController;
		TArray<AActor*> Waypoints;
		bool WaypointVisited;
		
		void RunState();
		void CheckPawns();

		//Listeners
		UFUNCTION()
		void OnDayStart(int DayLengthSeconds);

		UFUNCTION()
		void OnNightStart();

		UFUNCTION()
		void OnPortalActivate();

		UFUNCTION()
		void OnPortalDeactivate();
		
	private:
		bool CompareActorsByLocation(const AActor& a1, const AActor& a2);

};
