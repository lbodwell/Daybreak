// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "DestructibleResource.generated.h"

UCLASS()
class DAYBREAK_API ADestructibleResource : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructibleResource();

	UFUNCTION()
		void Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);
	void Destroy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
		class UDestructibleComponent* DestructibleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		bool IsDestroyed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		bool GotResources;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		AActor* OtherActorRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float DefaultDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float DefaultImpulse;
		
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Explode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
