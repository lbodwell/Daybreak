// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyState.h"
#include "DaybreakAIController.h"

/**
 * Add all states in here. They will be singletons
 */
class DAYBREAK_API Idle : public EnemyState
{
public:
	void enter(DaybreakAIController* controller) {}
	void transition(DaybreakAIController* controller) {}
	void exit(DaybreakAIController* controller) {}
	static EnemyState& getInstance();

private:
	Idle() {}
	Idle(const Idle& other);
	Idle& operator=(const Idle& other);
};


class DAYBREAK_API Patrol : public EnemyState
{
public:
	void enter(DaybreakAIController* controller) {}
	void transition(DaybreakAIController* controller) {}
	void exit(DaybreakAIController* controller) {}
	static EnemyState& getInstance();

private:
	Patrol() {}
	Patrol(const Patrol& other);
	Patrol& operator=(const Patrol& other);
};
