// Fill out your copyright notice in the Description page of Project Settings.

#include "DaybreakEnemyCharacter.h"
#include "EnemySpawnController.h"
#include "DaybreakAIController.h"
#include "Engine.h"
#include "DaybreakGameMode.h"

// Sets default values
ADaybreakEnemyCharacter::ADaybreakEnemyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 20;
	AttackDamage = 10;
	canReceiveDamage = true;
	canGiveDamage = true;
	Attacking = false;
	IsAlive = true;

	player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

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

// Called by AnimNotify::AttackFarthestReach in AnimBP
void ADaybreakEnemyCharacter::GiveDamage() {

	float capsuleRadius = 35;
	float distanceToPlayer = (GetActorLocation() - player->GetActorLocation()).Size() - capsuleRadius * 2;
	float distanceToPortal = dynamic_cast<ADaybreakAIController*>(GetController())->GetDistanceToPortal();
	
	if (Attacking && canGiveDamage) {
		if (distanceToPlayer < 40) {
			if (player != nullptr) {
				player->ReceiveDamage(AttackDamage);
			}
		}
		if (distanceToPortal < 100) {
			dynamic_cast<ADaybreakGameMode*>(UGameplayStatics::GetGameMode(GetWorld()))->DamagePortal(AttackDamage);
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

	int enemyCount = --AEnemySpawnController::EnemyCount;
	UE_LOG(LogActor, Warning, TEXT("Enemy Count: %d"), enemyCount);

	if (DayNightController && !DayNightController->GetIsDay()) {
		float value = AEnemySpawnController::EnemyValue;
		DayNightController->AddRotation(value);
		UE_LOG(LogActor, Warning, TEXT("Progressing night by: %f"), value);
		UE_LOG(LogActor, Warning, TEXT("New rotation: %f"), DayNightController->CurrentRotation);
	}

	GetController()->Destroy();
	GetMesh()->SetSimulatePhysics(true);

	((UPrimitiveComponent*)GetRootComponent())->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ADaybreakEnemyCharacter::DestroyCharacter() {
	Destroy();
}
