// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindingWaypoint.generated.h"

UCLASS()
class DAYBREAK_API APathfindingWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindingWaypoint();

	FORCEINLINE class USphereComponent* GetWaypointSphere() const { return WaypointSphere; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Waypoint", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* WaypointSphere;

};
