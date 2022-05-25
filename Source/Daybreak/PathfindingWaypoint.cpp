// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingWaypoint.h"
#include "Components/SphereComponent.h"

// Sets default values
APathfindingWaypoint::APathfindingWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WaypointSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WaypointSphere"));
	RootComponent = WaypointSphere;

}

// Called when the game starts or when spawned
void APathfindingWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
}


