// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStates.h"

EnemyState& Idle::getInstance()
{
	static Idle singleton;
	return singleton;
}

void Idle::run(ADaybreakAIController* controller)
{
	
}

EnemyState& Patrol::getInstance()
{
	static Patrol singleton;
	return singleton;
}

void Patrol::run(ADaybreakAIController* controller)
{
	
}

EnemyState& ChasePlayer::getInstance()
{
	static ChasePlayer singleton;
	return singleton;
}

void ChasePlayer::run(ADaybreakAIController* controller)
{
	
}

EnemyState& Attack::getInstance()
{
	static Attack singleton;
	return singleton;
}

void Attack::run(ADaybreakAIController* controller)
{
	
}
