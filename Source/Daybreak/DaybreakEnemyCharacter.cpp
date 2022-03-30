// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakEnemyCharacter.h"

// Sets default values
ADaybreakEnemyCharacter::ADaybreakEnemyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Attacking = false;
}

// Called when the game starts or when spawned
void ADaybreakEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ADaybreakEnemyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADaybreakEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADaybreakEnemyCharacter::Attack() {
	if (AttackMontage && !Attacking) {
		float const duration = PlayAnimMontage(AttackMontage, 1, NAME_None);
        if (duration > 0.f) {
            Attacking = true; // will be unset by AnimNotify::AttackHitEnd in AnimBP
        }
	}
}

bool ADaybreakEnemyCharacter::GetAttacking() {
	return Attacking;
}
