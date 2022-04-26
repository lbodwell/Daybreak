// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stdbool.h>

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.generated.h"

UCLASS()
class DAYBREAK_API ADayNightCycle : public AActor {
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDayStart, int, DayLengthSeconds);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNightStart);

public:
    // Sets default values for this actor's properties
    ADayNightCycle();
	
	// update sky position by specified angle
	UFUNCTION()
	void AddRotation(float angle);

    bool GetIsDay();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category=Sky)
    int DayLengthSeconds;

    public:
    UPROPERTY(EditAnywhere, Category=Sky)
    ADirectionalLight* Sun;

    UPROPERTY(EditAnywhere, Category=Sky)
    ADirectionalLight* Moon;

    UPROPERTY(EditAnywhere, Category=Sky)
    ASkyLight* SkyLight;

    // The current rotation of the sun in degrees (0-180 for night, 180-360 for day).
    float CurrentRotation;

    // get seconds remaining in day (0 at night)
    float GetDayLengthSecondsRemaining();

    int GetDayLengthSeconds();

    UPROPERTY(BlueprintAssignable)
    FDayStart OnDayStart;

    UPROPERTY(BlueprintAssignable)
    FNightStart OnNightStart;

private:
	/**
	* Sets the time to the specified angle relative to the position of the sun.
	* @param Angle  The angle in degrees to set the sun's position to (0-180 for night, 180-360 for day.)
	* @return  true if successful, otherwise false.
	*/
	void SetRotation(float angle);

	void BeginAutoRotation();

	// rate at which UpdateRotation is called
	float tickRate;

	// amount of sky rotation per tick
	float tickRotation;

	FTimerHandle autoRotateTimerHandle;
};