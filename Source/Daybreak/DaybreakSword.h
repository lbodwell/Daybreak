// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "DaybreakSword.generated.h"

USTRUCT()
struct FSwordLevel {
    GENERATED_BODY()

    UPROPERTY()
    int Index;

    UPROPERTY()
    FString Name;
	
	UPROPERTY()
	FLinearColor Color;

    UPROPERTY()
    int Damage;

    UPROPERTY()
    int Speed;
	
	UPROPERTY()
    int Range;

    FSwordLevel() {
        Index = 0;
        Name = "Steel Sword";
		Color = FLinearColor(0.2, 0.3, 1, 1);
        Damage = 0;
        Speed = 0;
        Range = 0;
    }
	
	FSwordLevel(int index, FString name, FLinearColor color, int damage, int speed, int range) {
        Index = index;
        Name = name;
		Color = color;
        Damage = damage;
        Speed = speed;
        Range = range;
    }
};

UCLASS()
class DAYBREAK_API ADaybreakSword : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADaybreakSword();
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	FSwordLevel CurrentLevel;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	TArray<struct FSwordLevel> Levels;
	
	void Upgrade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
