// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyState.h"
#include "DaybreakAIController.h"

/**
 * Add all states in here. 
 */

/**
*	DAYTIME STATES
*/

class DAYBREAK_API Daytime : public EnemyState {
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	Daytime() {}

private:

};



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


class DAYBREAK_API ChasePlayerDay : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	ChasePlayerDay() {}

private:

};


/**
*	NEUTRAL STATES
*/

class DAYBREAK_API AttackPlayer : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller);
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	AttackPlayer() {}

private:
	bool IsDay = true;
};


/**
*	NIGHTTIME STATES
*/

/**
*	NIGHTTIME
*/

class DAYBREAK_API Nighttime : public EnemyState {
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	Nighttime() {}

private:

};


/**
*	Visit Waypoint
*/

class DAYBREAK_API VisitWaypoint : public EnemyState {
public:
	void enter(ADaybreakAIController* controller);
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	VisitWaypoint() {}

private:
	FVector WaypointLocation;
};


/**
*	SWARM PORTAL
*/

class DAYBREAK_API SwarmPortal : public EnemyState {
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	SwarmPortal() {}

private:
	FVector PortalLocation;
};


/**
*	ATTACK PORTAL
*/

class DAYBREAK_API AttackPortal : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	AttackPortal() {}

private:

};


/**
*	Chase Player Night
*/

class DAYBREAK_API ChasePlayerNight : public EnemyState
{
public:
	void enter(ADaybreakAIController* controller) {}
	void run(ADaybreakAIController* controller);
	void exit(ADaybreakAIController* controller) {}
	ChasePlayerNight() {}

private:

};


//Helper Function
bool PlayerDistanceCheck(ADaybreakAIController* controller, float acceptableDistance, EnemyState* newState);


