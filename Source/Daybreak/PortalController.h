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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPortalActivate);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPortalDeactivate);

public:	
	// Sets default values for this component's properties
	UPortalController();

	UPROPERTY(BlueprintAssignable)
	FPortalActivate OnPortalActivate;

	UPROPERTY(BlueprintAssignable)
	FPortalDeactivate OnPortalDeactivate;

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
