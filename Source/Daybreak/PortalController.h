// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DayNightCycle.h"
#include "PortalController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAYBREAK_API UPortalController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPortalController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<ADayNightCycle> DayNightController;

	UPROPERTY(EditAnywhere)
	int MaxActivationsPerNight;
	UPROPERTY(EditAnywhere)
	int ActivationDurationSeconds;

	bool IsActive;

private:
	void PortalTick();

	int numActivationsThisNight;
	int timeInactiveSeconds;
	int timeActiveSeconds;
};
