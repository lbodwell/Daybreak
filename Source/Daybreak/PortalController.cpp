// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalController.h"
#include "UObject/ConstructorHelpers.h"
#include "DaybreakGameMode.h"

// Sets default values for this component's properties
UPortalController::UPortalController() {
	PrimaryComponentTick.bCanEverTick = false;
	IsActive = false;
	numActivationsThisNight = 0;

	static ConstructorHelpers::FObjectFinder<USoundCue> portalActivationCueObj(TEXT("SoundCue'/Game/Audio/Portal_Activation_Cue.Portal_Activation_Cue'"));
	if (portalActivationCueObj.Succeeded()) {
		PortalActivationCue = portalActivationCueObj.Object;
		portalActivationSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PortalActivationSound"));
	}
}

// Called when the game starts
void UPortalController::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle portalTickTimer;
	GetWorld()->GetTimerManager().SetTimer(portalTickTimer, this, &UPortalController::PortalTick, 1, true);
	UpdatePortalEffect();
	UpdatePortalEffect();

	if (portalActivationSound && PortalActivationCue) {
		portalActivationSound->SetSound(PortalActivationCue);
	}
}

void UPortalController::PortalTick() {
	if (DayNightController) {
		if (DayNightController->GetIsDay()) {
			IsActive = false;
			timeActiveSeconds = 0;
			timeInactiveSeconds = 0;
			numActivationsThisNight = 0;
		} else {
			if (IsActive) {
				if (timeActiveSeconds >= ActivationDurationSeconds) {
					UE_LOG(LogTemp, Warning, TEXT("Portal Deactivated"));
					// Broadcast portal deactivated here
					OnPortalDeactivate.Broadcast();
					UpdatePortalEffect();
					IsActive = false;
					timeActiveSeconds = 0;
				} else {
					timeActiveSeconds++;
				}
			// Ensure we haven't reached maximum portal activations for the night.
			// If it's night but no enemies are alive, we must be transitioning into next day. Don't allow portal to activate.
			} else if (numActivationsThisNight < MaxActivationsPerNight && ADaybreakGameMode::EnemyCount > 0) {
				float enemiesAtNightStart = 180.0f / ADaybreakGameMode::EnemyValue;

				// Chance of portal activating increases the more enemies are killed.
				// Once half of enemies are killed, probability has increased by 0.25.
				float killPity = (enemiesAtNightStart - ADaybreakGameMode::EnemyCount) / (float)(2 * enemiesAtNightStart);
				//UE_LOG(LogTemp, Warning, TEXT("kill pity: %f"), killPity);

				// Chance of portal activating is higher when fewer activations have already occured.
				// When no portal activations have occured yet, probability is increased at 0.1.
				float amountPity = (MaxActivationsPerNight - numActivationsThisNight) / (float)(10 * MaxActivationsPerNight);
				//UE_LOG(LogTemp, Warning, TEXT("amt pity: %f"), amountPity);

				// Chance of portal activating is higher the longer it's been since the last activation.
				// Once a full activation duration has elapsed, probability has increased by 0.25.
				float timePity = timeInactiveSeconds / (float)(4 * ActivationDurationSeconds);
				//UE_LOG(LogTemp, Warning, TEXT("time pity: %f"), timePity);

				// Combine the three "pity" metrics to get the probability of activating the portal, clamped at 100%.
				float activationProbability = FMath::Min(killPity + amountPity + timePity, 1.0f);
				//UE_LOG(LogTemp, Warning, TEXT("prob: %f"), activationProbability);
				float rand = FMath::FRandRange(0, 1);
				//UE_LOG(LogTemp, Warning, TEXT("rand: %f"), rand);

				if (rand < activationProbability) {
					UE_LOG(LogTemp, Warning, TEXT("Portal Activated"));
					// Broadcast portal activated here
					OnPortalActivate.Broadcast();
					UpdatePortalEffect();
					IsActive = true;
					if (portalActivationSound) {
						portalActivationSound->Play(0);
					}
					timeInactiveSeconds = 0;
					numActivationsThisNight++;
				} else {
					timeInactiveSeconds++;
				}
			}
		}
	}
}

 void UPortalController::UpdatePortalEffect() {
	 TArray<UActorComponent*> portalPlanes = GetOwner()->GetComponentsByTag(UActorComponent::StaticClass(), FName("PortalPlane"));
	 if (portalPlanes[0]) {
		portalPlanes[0]->ToggleActive();
	 }
 } 