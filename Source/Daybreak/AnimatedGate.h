// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DayNightCycle.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimatedGate.generated.h"

UCLASS()
class DAYBREAK_API AAnimatedGate : public AActor
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	ADayNightCycle* DayNightCycle;

	// Root component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimatedGate")
	class USceneComponent* RootScene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimatedGate")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "AnimatedGate")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "AnimatedGate")
	float MaxHeight;

	UPROPERTY(EditAnywhere, Category = "AnimatedGate")
	float MinHeight;

	UPROPERTY(EditAnywhere, Category = "AnimatedGate")
	float Direction;

public:	
	// Sets default values for this actor's properties
	AAnimatedGate();
	virtual void BeginPlay() override;
};
