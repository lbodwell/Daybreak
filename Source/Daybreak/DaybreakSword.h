// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DaybreakEnemyCharacter.h"
#include <vector>
#include "DaybreakSword.generated.h"

USTRUCT(BlueprintType)
struct FSwordLevel {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int Index;

    UPROPERTY(BlueprintReadOnly)
    FString Name;
	
	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color;

    UPROPERTY(BlueprintReadOnly)
    float Damage;

    UPROPERTY(BlueprintReadOnly)
    float Speed;
	
	UPROPERTY(BlueprintReadOnly)
    float Range;
	
	UPROPERTY(BlueprintReadOnly)
    int Cost;

    FSwordLevel() {
        Index = 0;
        Name = "Steel Sword";
		Color = FLinearColor(0.2, 0.3, 1, 1);
        Damage = 0;
        Speed = 0;
        Range = 0;
		Cost = 0;
    }
	
	FSwordLevel(int index, FString name, FLinearColor color, float damage, float speed, float range, int cost) {
        Index = index;
        Name = name;
		Color = color;
        Damage = damage;
        Speed = speed;
        Range = range;
		Cost = cost;
    }
};

UCLASS()
class DAYBREAK_API ADaybreakSword : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADaybreakSword();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FSwordLevel CurrentLevel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool Hitting;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateEffect();

	UFUNCTION()
	void Attack(class AActor* overlappedActor, class AActor* otherActor);
		
	TArray<struct FSwordLevel> Levels;
	
	void Upgrade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
