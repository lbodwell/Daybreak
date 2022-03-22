// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleResource.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
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

	MaxHealth = 10.f;

	DefaultDamage = 1.f;
	DefaultImpulse = 1.f;
}

// Called when the game starts or when spawned
void ADestructibleResource::BeginPlay() {
	Super::BeginPlay();

	DestructibleComponent->OnComponentHit.AddDynamic(this, &ADestructibleResource::Damage);

	CurrentHealth = MaxHealth;
}

// Called every frame
void ADestructibleResource::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called whenever damage is dealt to the Destructible Resource
void ADestructibleResource::Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);

	// Adjust this using math and the sword to ensure the player can only mine when the sword hits the resource.
	if (!IsDestroyed && OtherComp->ComponentHasTag("Weapon")) {
		CurrentHealth -= 1.f;

		if (CurrentHealth <= 0.f) {
			Destroy();
		}
	}

}

// Called whenever the destructible resource is completely destroyed
void ADestructibleResource::Destroy() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, __FUNCTION__);
}

