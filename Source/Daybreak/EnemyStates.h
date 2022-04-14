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
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	Idle() {}

private:

};


class DAYBREAK_API Patrol : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller);
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	Patrol() {}

private:
	FVector Destination;
};


class DAYBREAK_API ChasePlayer : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	ChasePlayer() {}

private:

};


class DAYBREAK_API Attack : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	Attack() {}

private:
	Attack(const Attack& other);
	Attack& operator=(const Attack& other);
};

