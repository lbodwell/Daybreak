// Copyright Epic Games, Inc. All Rights Reserved.

#include "DaybreakGameMode.h"
#include "DaybreakCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleResource.h"
#include "DaybreakHUD.h"
#include "Math/UnrealMathUtility.h"
#include "DayNightCycle.h"


int ADaybreakGameMode::EnemyCount = 0;
float ADaybreakGameMode::EnemyValue = 0;

ADaybreakGameMode::ADaybreakGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Main_Character/MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	EnemyCount = 0;
	EnemyValue = 0;
	PortalHealth = 500;
	tooltipStage = 0;
}


void ShuffleAndRemove(TArray<AActor*> ResourcesToShuffle) {

	// Randomly shuffle arrays
	for (int32 i = ResourcesToShuffle.Num() - 1; i > 0; i--) {
		int32 j = FMath::Floor(FMath::Rand() * (i + 1)) % ResourcesToShuffle.Num();
		AActor* temp = ResourcesToShuffle[i];
		ResourcesToShuffle[i] = ResourcesToShuffle[j];
		ResourcesToShuffle[j] = temp;
	}

	// "Remove" the first half (number can be adjusted) of the Resources in the TArray
	for (int i = 0; i < ResourcesToShuffle.Num() / 2; i++) {
		if (!ResourcesToShuffle[i]->ActorHasTag("Tutorial")) {
			ResourcesToShuffle[i]->Destroy();
			//ResourcesToShuffle[i]->SetActorHiddenInGame(true);
			//ResourcesToShuffle[i]->SetActorEnableCollision(false);
		}
	}
}


void ADaybreakGameMode::BeginPlay() {
	Super::BeginPlay();
	
	// get portal
	TArray<AActor*> portals;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Portal"), portals);
	portal = portals[0];
	
	// get day/night controller
	TArray<AActor*> dayNightCycles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightCycle::StaticClass(), dayNightCycles);
	dayNightController = Cast<ADayNightCycle>(dayNightCycles[0]);
	
	// get player
	player = Cast<ADaybreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// add HUD widget to viewport
	HUD = Cast<UDaybreakHUD>(CreateWidget<UUserWidget>(GetWorld(), HUDWidget));
	if (HUD) {
		HUD->AddToViewport();
	}
	
	// get player controller
	APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	// disable mouse
	playerController->bShowMouseCursor = false;
	playerController->bEnableClickEvents = false;
	playerController->bEnableMouseOverEvents = false;
	
	FInputModeGameOnly inputMode;
	playerController->SetInputMode(inputMode);

	// Find all Resources and compile them into a TArray
	TArray<AActor*> AllResources;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleResource::StaticClass(), AllResources);

	TArray<AActor*> ResourcesLevel1;
	TArray<AActor*> ResourcesLevel2;
	TArray<AActor*> ResourcesLevel3;

	// Separate all resources based on level
	for (int32 i = 0; i < AllResources.Num(); i++) {
		if (AllResources[i]->ActorHasTag("Tutorial")) {
			tutorialDarkstone = AllResources[i];
			continue;
		}
		
		ADestructibleResource* Resource = Cast<ADestructibleResource>(AllResources[i]);

		if (Resource->Level == 1) {
			ResourcesLevel1.Add(AllResources[i]);
		}
		else if (Resource->Level == 2) {
			ResourcesLevel2.Add(AllResources[i]);
		}
		else if (Resource->Level == 3) {
			ResourcesLevel3.Add(AllResources[i]);
		}
	}

	// Shuffle Resources by level & Remove Some of Each Set
	ShuffleAndRemove(ResourcesLevel1);
	ShuffleAndRemove(ResourcesLevel2);
	ShuffleAndRemove(ResourcesLevel3);
	
	// begin tutorial update timer
	GetWorld()->GetTimerManager().SetTimer(TooltipUpdateHandle, this, &ADaybreakGameMode::UpdateTooltip, 1, true);
}

void ADaybreakGameMode::DamagePortal(int DamageAmount) {
	PortalHealth -= DamageAmount;

	if (PortalHealth <= 0) {
		//kill player
		player->Destroy();
	}
}

float ADaybreakGameMode::GetDistanceToPlayer(FVector point) {
	return (point - player->GetActorLocation()).Size() - 70;
}

float ADaybreakGameMode::GetDistanceToPortal(FVector point) {
	return (point - portal->GetActorLocation()).Size() - 70;
}

UDaybreakHUD* ADaybreakGameMode::GetHUD() {
	return HUD;
}

void ADaybreakGameMode::TriggerDarkstoneTutorial() {
	tooltipStage = 1;
	if (HUD) {
		HUD->AddMessage("Gather Darkstone until sun sets...");
		
		// outline tutorial darkstone
		if (tutorialDarkstone) {
			TArray<UActorComponent*> components = tutorialDarkstone->GetComponentsByTag(UActorComponent::StaticClass(), "Outline");
			if (components.Num() > 0) {
				Cast<UPrimitiveComponent>(components[0])->SetRenderCustomDepth(true);
			}
		}
	}	
}

void ADaybreakGameMode::TriggerAnvilTutorial() {
	tooltipStage = 2;
	if (HUD) {
		HUD->AddMessage("Upgrade your equipment before the horde arrives...");
		
		// outline anvil
		TArray<AActor*> anvils;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Anvil"), anvils);
		if (anvils.Num() > 0) {
			TArray<UActorComponent*> components = anvils[0]->GetComponentsByTag(UActorComponent::StaticClass(), "Outline");
			if (components.Num() > 0) {
				Cast<UPrimitiveComponent>(components[0])->SetRenderCustomDepth(true);
			}
		}
	}	
}

void ADaybreakGameMode::TriggerPortalTutorial() {
	tooltipStage = 3;
	if (HUD) {
		HUD->AddMessage("Defend the portal at all costs!");
		
		// outline portal
		if (portal) {
			TArray<UActorComponent*> components = portal->GetComponentsByTag(UActorComponent::StaticClass(), "Outline");
			if (components.Num() > 0) {
				Cast<UPrimitiveComponent>(components[0])->SetRenderCustomDepth(true);
			}
		}
	}	
}

void ADaybreakGameMode::UpdateTooltip() {
	// wait to progress tutorial until:
	//   - the day has progressed 10 seconds
	if (tooltipStage == 0 && dayNightController->GetDayLengthSeconds() - dayNightController->GetDayLengthSecondsRemaining() >= 10) {
		TriggerDarkstoneTutorial();
	}
	// wait to progress tutorial until:
	//   - the player collects 600 Darkstone
	//   - or there is only 60 seconds remaining in the day
	else if (tooltipStage == 1 && (player->DarkStone >= 600 || dayNightController->GetDayLengthSecondsRemaining() < 60)) {
		TriggerAnvilTutorial();
	}
	// wait to progress tutorial until:
	//   - the first day has ended
	else if (tooltipStage == 2 && dayNightController->GetDayLengthSecondsRemaining() == 0) {
		TriggerPortalTutorial();
	}
	// wait to progress tutorial until:
	//   - the first night has ended
	else if (tooltipStage == 3 && dayNightController->GetDayLengthSecondsRemaining() > 0 && HUD) {
		HUD->RemoveMessage();
	}
}