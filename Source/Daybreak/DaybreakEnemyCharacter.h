// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DaybreakCharacter.h"
#include "DayNightCycle.h"
#include "DaybreakEnemyCharacter.generated.h"

class ADaybreakCharacter;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
    UAnimMontage* HitReactionMontage;

	/** Whether enemy is currently attacking and should not be able to attack again yet. */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=State)
	bool Attacking;
	
	bool canReceiveDamage;
	bool canGiveDamage;

	void KillCharacter(float CorpsePersistenceTime);
	void DestroyCharacter();

	class ADaybreakCharacter* player;

public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<ADayNightCycle> DayNightController;

	void Attack();

	UFUNCTION(BlueprintCallable, Category=Attack)
	void GiveDamage();
	
	bool GetAttacking();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReceiveDamage(int DamageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	int Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	int AttackDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	float CapsuleRadius;

	bool IsAlive;

};
