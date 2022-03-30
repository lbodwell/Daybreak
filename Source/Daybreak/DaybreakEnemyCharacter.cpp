// Fill out your copyright notice in the Description page of Project Settings.


#include "DaybreakEnemyCharacter.h"

// Sets default values
ADaybreakEnemyCharacter::ADaybreakEnemyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 20;
	Attacking = false;
	IsAlive = true;
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

void ADaybreakEnemyCharacter::ReceiveDamage(int DamageAmount) {
	Health -= DamageAmount;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::FromInt(Health));

	if (Health <= 0 && IsAlive) {
		KillCharacter(5.f);
	}
}

bool ADaybreakEnemyCharacter::GetAttacking() {
	return Attacking;
}

void ADaybreakEnemyCharacter::KillCharacter(float CorpsePersistanceTime) {
	UE_LOG(LogTemp, Warning, TEXT("Killing enemy in 5 seconds"));
	IsAlive = false;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADaybreakEnemyCharacter::DestroyCharacter, 0.1, false, CorpsePersistanceTime);

	GetController()->UnPossess();
	GetMesh()->SetSimulatePhysics(true);

	((UPrimitiveComponent*)GetRootComponent())->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ADaybreakEnemyCharacter::DestroyCharacter() {
	UE_LOG(LogTemp, Warning, TEXT("Destroying Enemy"));
	Destroy();
}
