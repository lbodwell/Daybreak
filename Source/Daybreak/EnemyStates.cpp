// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStates.h"

EnemyState& Idle::getInstance()
{
	static Idle singleton;
	return singleton;
}

void Idle::transition(DaybreakAIController* controller)
{
	
}

EnemyState& Patrol::getInstance()
{
	static Patrol singleton;
	return singleton;
}

void Patrol::transition(DaybreakAIController* controller)
{
	
}

EnemyState& ChasePlayer::getInstance()
{
	static ChasePlayer singleton;
	return singleton;
}

void ChasePlayer::transition(DaybreakAIController* controller)
{
	// TODO: insert return statement here
}

EnemyState& Attack::getInstance()
{
	static Attack singleton;
	return singleton;
}

void Attack::transition(DaybreakAIController* controller)
{
	// TODO: insert return statement here
}
