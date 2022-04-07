// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DaybreakEnemyCharacter.h"
#include "EnemyState.h"
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
		ADaybreakAIController();
		
		virtual void BeginPlay() override;
		
	protected:
		ADaybreakEnemyCharacter* pawn;
		AActor* playerActor;
		
		void ChasePlayer();
};
