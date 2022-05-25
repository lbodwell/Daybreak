// Fill out your copyright notice in the Description page of Project Settings.

#include "DaybreakEnemyCharacter.h"
#include "Engine.h"
#include "DaybreakGameMode.h"

// Sets default values
ADaybreakEnemyCharacter::ADaybreakEnemyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 20;
	canReceiveDamage = true;
	canGiveDamage = true;
	Attacking = false;
	IsAlive = true;
}

// Called when the game starts or when spawned
void ADaybreakEnemyCharacter::BeginPlay() {
	Super::BeginPlay();

	player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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

// Called by AnimNotify::AttackFarthestReach in AnimBP
void ADaybreakEnemyCharacter::GiveDamage() {
	ADaybreakGameMode* gameMode = Cast<ADaybreakGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	float distanceToPlayer = gameMode->GetDistanceToPlayer(GetActorLocation());
	float distanceToPortal = gameMode->GetDistanceToPortal(GetActorLocation());
	
	if (Attacking && canGiveDamage) {
		if (distanceToPlayer < CapsuleRadius) {
			if (player != nullptr) {
				player->ReceiveDamage(AttackDamage);
			}
		}
		if (distanceToPortal < 200) {
			gameMode->DamagePortal(AttackDamage);
		}

	}
}

void ADaybreakEnemyCharacter::ReceiveDamage(int DamageAmount) {
	if (!canReceiveDamage) return;

	Health -= DamageAmount;

	if (Health <= 0 && IsAlive) {
        KillCharacter(60.f);
	} else {
		if (HitReactionMontage) {
			PlayAnimMontage(HitReactionMontage, 1, NAME_None);
		}

		canReceiveDamage = false;
		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, [&]() { canReceiveDamage = true; }, 0.2, false, 0.2);
	}
}

bool ADaybreakEnemyCharacter::GetAttacking() {
	return Attacking;
}

void ADaybreakEnemyCharacter::KillCharacter(float CorpsePersistenceTime) {
	IsAlive = false;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADaybreakEnemyCharacter::DestroyCharacter, 0.1, false, CorpsePersistenceTime);

	int enemyCount = --ADaybreakGameMode::EnemyCount;
	UE_LOG(LogActor, Warning, TEXT("Enemy Count: %d"), enemyCount);

	if (DayNightController && !DayNightController->GetIsDay()) {
		float value = ADaybreakGameMode::EnemyValue;
		DayNightController->AddRotation(value);
		//UE_LOG(LogActor, Warning, TEXT("Progressing night by: %f"), value);
	}

	GetController()->Destroy();
	GetMesh()->SetSimulatePhysics(true);

	((UPrimitiveComponent*)GetRootComponent())->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ADaybreakEnemyCharacter::DestroyCharacter() {
	Destroy();
}
