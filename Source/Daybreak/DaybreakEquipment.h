#pragma once

#include "DaybreakEquipment.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDaybreakEquipment : public UInterface {
    GENERATED_BODY()
};

class IDaybreakEquipment {    
    GENERATED_BODY()
	
private:
	bool IsUpgrading;
	float UpgradeProgress;

public:
	IDaybreakEquipment();

	float GetUpgradeProgress();
	
	bool GetIsUpgrading();
	
	void SetIsUpgrading(bool isUpgrading);
	
	/**
	Increase progress towards upgrading to next level.
	Returns true if sword upgrades and progress is reset to 0.
	*/
    bool IncreaseUpgradeProgress(float increase);
	
	/**
	Decreases progress towards upgrading to next level.
	Returns true if sword progress reaches 0.
	*/
	bool DecreaseUpgradeProgress(float decrease);
	
	virtual void Upgrade() = 0;
};