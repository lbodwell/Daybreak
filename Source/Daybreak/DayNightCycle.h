// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stdbool.h>

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.generated.h"

UCLASS()
class DAYBREAK_API ADayNightCycle : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayNightCycle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Advances time by the specified angle relative to the position of the sun.
	* @param Angle  The angle in degrees to move the sun's position by (0-180 for night, 180-360 for day).
	*/
	void AdvanceTime(float Angle);

	/**
	* Extends the current day by rewinding time by the specified angle relative to the position of the sun.
	* @param Angle  The angle in degrees to move the sun's position by (0-180 for night, 180-360 for day).
	*/
	void RewindTime(float Angle);

	/**
	* Gets the current time in hours based on the rotation of the sun.
	*/
	float GetTimeInHours();

	UPROPERTY(EditAnywhere, Category = Sky)
	AActor* Sun;

	UPROPERTY(EditAnywhere, Category = Sky)
	ADirectionalLight* LightSource;
	
	// Returns the current rotation of the sun in degrees (0-180 for night, 180-360 for day).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sky)
	float CurrentRotation;

	// Returns true if it is currently daytime, false if it night-time.
	bool IsDayTime;

	// Temporary variable that needs to be in the global scope for timeout-based testing.
	int TestAlternator;
};
