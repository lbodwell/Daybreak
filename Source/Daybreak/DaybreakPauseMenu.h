// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DaybreakPauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class DAYBREAK_API UDaybreakPauseMenu : public UUserWidget {
	GENERATED_BODY()
	
	protected:
		virtual bool Initialize() override;
		virtual void NativeConstruct() override;
		virtual void NativeDestruct() override;
};
