// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DaybreakSword.h"
#include "DaybreakArmor.h"
#include "DaybreakCharacter.h"
#include "DaybreakUpgradeMenu.generated.h"

UCLASS()
class DAYBREAK_API UDaybreakUpgradeMenu : public UUserWidget {
	GENERATED_BODY()
	
	DECLARE_DELEGATE_OneParam(FOnUpgrade, IDaybreakEquipment*);
	
public:
	// sword getters for UI
	UFUNCTION(BlueprintCallable)
	FSwordLevel GetCurrentSwordLevel();
		
	UFUNCTION(BlueprintCallable)
	FSwordLevel GetNextSwordLevel();
		
	UFUNCTION(BlueprintCallable)
	float GetSwordUpgradeProgress();
		
	// armor getters for UI
	UFUNCTION(BlueprintCallable)
	FArmorLevel GetCurrentArmorLevel();
		
	UFUNCTION(BlueprintCallable)
	FArmorLevel GetNextArmorLevel();
		
	UFUNCTION(BlueprintCallable)
	float GetArmorUpgradeProgress();
	
	UFUNCTION(BlueprintCallable)
	float GetDarkStoneAvailable();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateUI();
		
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
		
	ADaybreakCharacter* player;
	ADaybreakSword* sword;
	ADaybreakArmor* armor;
		
	TArray<IDaybreakEquipment*> equipmentBeingUpgraded;
		
	FTimerHandle upgradeTimerHandle;
				
	void StartUpgrading(IDaybreakEquipment* equipment);
	void StopUpgrading(IDaybreakEquipment* equipment);
};
