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

	void AdvanceTime(float Amount);
	void RewindTime(float Amount);

	UPROPERTY(EditAnywhere, Category = "Sky")
	AActor* Sun;

	UPROPERTY(EditAnywhere, Category = "Sky")
	ADirectionalLight* LightSource;

	UPROPERTY(EditAnywhere, Category = "Sky")
	float TurnRate;

	bool IsDayTime;
};
