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
	static EnemyState& getInstance();

private:
	Idle() {}
	Idle(const Idle& other);
	Idle& operator=(const Idle& other);
};


class DAYBREAK_API Patrol : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	static EnemyState& getInstance();

private:
	Patrol() {}
	Patrol(const Patrol& other);
	Patrol& operator=(const Patrol& other);
};


class DAYBREAK_API ChasePlayer : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	static EnemyState& getInstance();

private:
	ChasePlayer() {}
	ChasePlayer(const ChasePlayer& other);
	ChasePlayer& operator=(const ChasePlayer& other);
};


class DAYBREAK_API Attack : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	static EnemyState& getInstance();

private:
	Attack() {}
	Attack(const Attack& other);
	Attack& operator=(const Attack& other);
};
