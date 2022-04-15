// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DaybreakEnemyCharacter.h"
#include "DestructibleResource.h"
#include <vector>
#include "DaybreakEquipment.h"
#include "DaybreakArmor.generated.h"

USTRUCT(BlueprintType)
struct FArmorLevel {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int Index;

    UPROPERTY(BlueprintReadOnly)
    FString Name;
	
	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color;

    UPROPERTY(BlueprintReadOnly)
    float Protection;
	
	UPROPERTY(BlueprintReadOnly)
    float KnockbackResistance;
	
	UPROPERTY(BlueprintReadOnly)
    float MovementSpeed;
	
	UPROPERTY(BlueprintReadOnly)
    float AttackSpeed;
	
	UPROPERTY(BlueprintReadOnly)
    int Cost;

    FArmorLevel() {
        Index = 0;
        Name = "Steel Armor";
		Color = FLinearColor(0.25, 0.25, 0.25, 1);
        Protection = 0;
        KnockbackResistance = 0;
		MovementSpeed = 0;
		AttackSpeed = 0;
		Cost = 0;
    }
	
	FArmorLevel(int index, FString name, FLinearColor color, float protection, float knockbackResistance, float movementSpeed, float attackSpeed, int cost) {
        Index = index;
        Name = name;
		Color = color;
        Protection = protection;
        KnockbackResistance = knockbackResistance;
		MovementSpeed = movementSpeed;
		AttackSpeed = attackSpeed;
		Cost = cost;
    }
};

UCLASS()
class DAYBREAK_API ADaybreakArmor : public AActor, public IDaybreakEquipment {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADaybreakArmor();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FArmorLevel CurrentLevel;
		
	TArray<struct FArmorLevel> Levels;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Upgrade();

};
