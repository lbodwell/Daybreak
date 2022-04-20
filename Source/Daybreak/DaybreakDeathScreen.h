// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DaybreakDeathScreen.generated.h"

/**
 * 
 */
UCLASS()
class DAYBREAK_API UDaybreakDeathScreen : public UUserWidget {
	GENERATED_BODY()

	protected:
		virtual bool Initialize() override;
		virtual void NativeConstruct() override;
		virtual void NativeDestruct() override;

		UFUNCTION(BlueprintCallable)
		void ExitMenu();
};