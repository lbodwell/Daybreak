// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DaybreakSword.h"
#include "Daybreak2DSwordCamera.generated.h"

UCLASS()
class DAYBREAK_API ADaybreak2DSwordCamera : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADaybreak2DSwordCamera();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateLevel(FSwordLevel level);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Variables)
	int ID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
