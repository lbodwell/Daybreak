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
	* Updates the interal current rotation based on the sun's rotation
	*/
	void UpdateRotation();

	UPROPERTY(EditAnywhere, Category = Sky)
	ADirectionalLight* Sun;

	UPROPERTY(EditAnywhere, Category = Sky)
	ADirectionalLight* Moon;
	
	// The current rotation of the sun in degrees (0-180 for night, 180-360 for day).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sky)
	float CurrentRotation;

	// Ttrue if it the sun's current rotation indicates that it is daytime and false if it is night-time.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sky)
	bool IsDayTime;

	/**
	* Advances time by the specified angle relative to the position of the sun.
	* @param Angle  The magnitude of the angle in degrees (0-360) to advance the sun's position by.
	* @return  true if successful, otherwise false.
	*/
	bool AdvanceTime(float Angle);

	/**
	* Extends the current day by rewinding time by the specified angle relative to the position of the sun.
	* @param Angle  The magnitude of the angle in degrees (0-360) to rewind the sun's position by.
	* @return  true if successful, otherwise false.
	*/
	bool RewindTime(float Angle);

	/**
	* Sets the time to the specified angle relative to the position of the sun.
	* @param Angle  The angle in degrees to set the sun's position to (0-180 for night, 180-360 for day.)
	* @return  true if successful, otherwise false.
	*/
	bool SetTime(float Angle);

	/**
	* Gets the current time in hours based on the rotation of the sun (15 degrees = 1 hour.)
	*/
	float GetTimeInHours();

private:
	// Temporary variable that needs to be in the global scope for timeout-based testing.
	int TestAlternator;
};
