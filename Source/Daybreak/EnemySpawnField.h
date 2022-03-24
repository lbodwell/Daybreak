// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <tuple>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnField.generated.h"
using namespace std;

UCLASS()
class DAYBREAK_API AEnemySpawnField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//Range Dimensions
	UPROPERTY(EditAnywhere)
	int FieldWidth;
	
	UPROPERTY(EditAnywhere)
		int FieldHeight;

	UPROPERTY(EditAnywhere)
	int FieldDepth;

	tuple<FVector, FVector> getSpawnFieldBoundaries();

};
