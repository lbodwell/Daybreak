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
	if (PlayerDistanceCheck(controller, 500, new ChasePlayerDay)) { return; }

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
	if (PlayerDistanceCheck(controller, 500, new ChasePlayerDay)) { return; }

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

void ChasePlayerDay::run(ADaybreakAIController* controller)
{
	//get player location
	float playerDist = controller->GetDistanceToPlayer();

	if (playerDist > 1000) { controller->SetState(new Patrol); return; }

	else if (PlayerDistanceCheck(controller, 40, new AttackPlayer)) { return; }

	else { controller->ChasePlayer(); }
	
}



/**
*	ATTACK PLAYER
*/
void AttackPlayer::enter(ADaybreakAIController* controller) {
	IsDay = controller->GetIsDay();
}

void AttackPlayer::run(ADaybreakAIController* controller)
{
	//if out of range transition to chase player
	if (controller->GetDistanceToPlayer() > 40) {
		if (IsDay) { controller->SetState(new ChasePlayerDay); return; }
		else { controller->SetState(new ChasePlayerNight); return; }
	}

	//otherwise attack player
	controller->Attack();

}

/**
*	NIGHTIME
*/

void Nighttime::run(ADaybreakAIController* controller) {
	controller->SetState(new ChasePlayerNight);
}


/**
*	Visit Waypoint
*/

void VisitWaypoint::enter(ADaybreakAIController* controller) {
	WaypointLocation = controller->GetRandomWaypoint();
}

void VisitWaypoint::run(ADaybreakAIController* controller) {
	PlayerDistanceCheck(controller, 250, new ChasePlayerNight);

	if (controller->GetWaypointIsVisited()) { controller->SetState(new SwarmPortal); }
	
	if (controller->MoveToLocation(WaypointLocation, 200.0f, true, true, true, true, NULL, true) == EPathFollowingRequestResult::AlreadyAtGoal) {
		controller->SetState(new SwarmPortal);
	}

}

void VisitWaypoint::exit(ADaybreakAIController* controller) {
	controller->SetWaypointVisited(true);
}


/**
*	SWARM PORTAL
*/

void SwarmPortal::run(ADaybreakAIController* controller) {
	
	if (controller->GetDistanceToPortal() < 200) {
		controller->SetState(new AttackPortal);
		return;
	}

	//else if (PlayerDistanceCheck(controller, 250, new ChasePlayerNight)) { return; }

	else {
		//run to portal
		if (controller->MoveToLocation(controller->GetPortalLocation(), 1.0f, true, true, true, true, NULL, true) == EPathFollowingRequestResult::AlreadyAtGoal) {
			controller->SetState(new AttackPortal);
		}
	}

}

/**
*	Attack Portal
*/

void AttackPortal::run(ADaybreakAIController* controller) {

	//UE_LOG(LogTemp, Warning, TEXT("In Attack Portal"));

	if (controller->GetDistanceToPortal() > 200) {
		controller->SetState(new SwarmPortal);
		return;
	}
	else if (PlayerDistanceCheck(controller, 100, new ChasePlayerNight)) { return; }
	else { controller->Attack(); }
}


/**
*	Chase Player Night
*/

void ChasePlayerNight::run(ADaybreakAIController* controller)
{
	//get player location
	//float playerDist = controller->GetDistanceToPlayer();

	//if (playerDist > 700) { controller->SetState(new SwarmPortal); return; }

	if (PlayerDistanceCheck(controller, 40, new AttackPlayer)) { return; }

	else { controller->ChasePlayer(); }

}


//Helper Functions
bool PlayerDistanceCheck(ADaybreakAIController* controller, float acceptableDistance, EnemyState* newState) {

	float playerDist = controller->GetDistanceToPlayer();
	if (playerDist < acceptableDistance) {
		controller->SetState(newState);
		return true;
	}

	return false;
}


