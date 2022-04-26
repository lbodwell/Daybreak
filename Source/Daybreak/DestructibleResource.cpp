// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleResource.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "DaybreakSword.h"
#include "Engine.h"
#include "DaybreakCharacter.h"

// Sets default values
ADestructibleResource::ADestructibleResource() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Dustructible Component"));
	DestructibleComponent->SetupAttachment(RootComponent);
	DestructibleComponent->SetNotifyRigidBodyCollision(true);

	IsDestroyed = false;
	GotResources = false;
}

// Called when the game starts or when spawned
void ADestructibleResource::BeginPlay() {
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

// Called every frame
void ADestructibleResource::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called when resources are damaged by sword swings
void ADestructibleResource::Damage(float damage) {
	CurrentHealth -= damage;


	if (CurrentHealth <= 0 && !IsDestroyed) {
		IsDestroyed = true;

		ADaybreakCharacter* player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		// add DarkStone
		if (player != nullptr) {
			player->DarkStone += DarkStoneFound;
		}

		Explode();
	}
}

 void ADestructibleResource::Explode_Implementation() {
 }