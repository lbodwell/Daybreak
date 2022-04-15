// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStates.h"

//might want to make player distance check helper function

/**
*	DAYTIME
*/

void Daytime::run(ADaybreakAIController* controller) {
	controller->SetState(new Idle);
}


/**
*	IDLE
*/

void Idle::run(ADaybreakAIController* controller)
{
	//check if player is nearby
	//if so, transition to chasePlayer
	if (controller->GetDistanceToPlayer() < 500) { controller->SetState(new ChasePlayer); return; }

	//otherwise
	//do nothing for an amount of time then transition to Patrol
	else {
		static int timer = FMath::RandRange(10, 50);
		if (timer > 0) {
			timer--;
		}
		else {
			timer = FMath::RandRange(10, 50);
			controller->SetState(new Patrol);
		}
	}
}


/**
*	PATROL
*/

void Patrol::enter(ADaybreakAIController* controller) {
	Destination = controller->GetRandomNearbyLocation();
}

void Patrol::run(ADaybreakAIController* controller)
{
	//check if player is nearby
	//if so, transition to chasePlayer
	if (controller->GetDistanceToPlayer() < 500) { controller->SetState(new ChasePlayer); return; }

	//otherwise move to random location, then idle
	else {		
		if (controller->MoveToLocation(Destination, 1.0f, true, true, true, true, NULL, true) == EPathFollowingRequestResult::AlreadyAtGoal || EPathFollowingRequestResult::Failed) {
			controller->SetState(new Idle);
		}
		
	} 
}



/**
*	CHASE PLAYER
*/


void ChasePlayer::run(ADaybreakAIController* controller)
{
	//get player location
	float playerDist = controller->GetDistanceToPlayer();

	if (playerDist > 1000) { controller->SetState(new Patrol); return; }

	else if (playerDist <= 40) { controller->SetState(new AttackPlayer); return; }

	else { controller->ChasePlayer(); }
	
}



/**
*	ATTACK PLAYER
*/


void AttackPlayer::run(ADaybreakAIController* controller)
{
	UE_LOG(LogTemp, Warning, TEXT("In Attack Player"));
	//if out of range transition to chase player
	if (controller->GetDistanceToPlayer() > 40) { controller->SetState(new ChasePlayer); return; }

	//otherwise attack player
	controller->Attack();

}

/**
*	NIGHTIME
*/

void Nighttime::run(ADaybreakAIController* controller) {
	controller->SetState(new SwarmPortal);
}


/**
*	SWARM PORTAL
*/

void SwarmPortal::run(ADaybreakAIController* controller) {
	
	if (controller->GetDistanceToPortal() < 10) {
		controller->SetState(new AttackPortal);
	}
	else if () {
		
	}

}

/**
*	Attack Portal
*/

void AttackPortal::run(ADaybreakAIController* controller) {

	if (controller->GetDistanceToPortal() > 10) {
		controller->SetState(new SwarmPortal);
	}
	else if (controller->GetDistanceToPlayer() < 100) {
		controller->SetState(new ChasePlayer);
	}
	else {
		controller->Attack();
	}
}


