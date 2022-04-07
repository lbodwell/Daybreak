// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStates.h"

//might want to make player distance check hierarchical

EnemyState& Idle::getInstance()
{
	static Idle singleton;
	return singleton;
}

void Idle::run(ADaybreakAIController* controller)
{
	//check if player is nearby
	//if so, transition to chasePlayer
	if (controller->GetDistanceToPlayer() < 100) {
		controller->SetState(ChasePlayer::getInstance());
		return;
	}
	//otherwise
	//do nothing for an amount of time then transition to Patrol
	else {
		static int timer = 20;
		if (timer > 0) {
			timer--;
		}
		else {
			timer = 20;
			controller->SetState(Patrol::getInstance());
		}
	}
}

EnemyState& Patrol::getInstance()
{
	static Patrol singleton;
	return singleton;
}

void Patrol::enter(ADaybreakAIController* controller) {
	Destination = controller->GetRandomNearbyLocation();
}

void Patrol::run(ADaybreakAIController* controller)
{
	//check if player is nearby
	//if so, transition to chasePlayer
	if (controller->GetDistanceToPlayer() < 100) {
		controller->SetState(ChasePlayer::getInstance());
		return;
	}
	else {
		if (controller->MoveToLocation(Destination, 0.0f) == EPathFollowingRequestResult::AlreadyAtGoal) {
			controller->SetState(Idle::getInstance());
		}
	} 
}

EnemyState& ChasePlayer::getInstance()
{
	static ChasePlayer singleton;
	return singleton;
}

void ChasePlayer::run(ADaybreakAIController* controller)
{
	//get player location
	float playerDist = controller->GetDistanceToPlayer();
	if (playerDist > 1000) {
		controller->SetState(Patrol::getInstance());
		return;
	}
	else if (playerDist < 40) {
		controller->SetState(Attack::getInstance());
	}
	else {
		
	}
	//move to player
	//if close enough, transition to attack
	//else if far enough transition to idle
}

EnemyState& Attack::getInstance()
{
	static Attack singleton;
	return singleton;
}

void Attack::run(ADaybreakAIController* controller)
{
	//get player distance
	//if close enough, attack
	//otherwise transition to chaseplayer
}

