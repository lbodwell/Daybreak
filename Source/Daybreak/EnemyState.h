// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaybreakAIController.h"

//Forward declaration to resolve circular dependency
class DaybreakAIController;

/**
 *	Pure abstract class interface for the enemy AI states
 */
class DAYBREAK_API EnemyState
{
public:
	virtual void enter(DaybreakAIController* controller) = 0;
	virtual void transition(DaybreakAIController* controller) = 0;
	virtual void exit(DaybreakAIController* controller) = 0;
	virtual ~EnemyState() {}
};
