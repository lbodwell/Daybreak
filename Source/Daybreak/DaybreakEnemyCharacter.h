// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DaybreakEnemyCharacter.generated.h"

UCLASS()
class DAYBREAK_API ADaybreakEnemyCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADaybreakEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* AttackMontage;
	
	/** Whether enemy is currently attacking and should not be able to attack again yet. */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=State)
	bool Attacking;

public:	
	void Attack();
	
	bool GetAttacking();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReceiveDamage(int DamageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	int Health;

};
