// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// may not need
#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "MusicController.generated.h"

UCLASS()
class DAYBREAK_API AMusicController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicController();

private:
	UAudioComponent* dayMusic;
	UAudioComponent* nightMusic;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<ADayNightCycle> DayNightController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundCue* DayMusicCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundCue* NightMusicCue;

	void OnDayStart(int dayLengthSeconds);
	void OnNightStart();
};
