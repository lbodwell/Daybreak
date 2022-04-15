
#include "DaybreakEquipment.h"

IDaybreakEquipment::IDaybreakEquipment() {
	IsUpgrading = false;
	UpgradeProgress = 0;
	UE_LOG(LogActor, Warning, TEXT("INITIALIZED!"));
}

float IDaybreakEquipment::GetUpgradeProgress() {
    return UpgradeProgress;
}

bool IDaybreakEquipment::GetIsUpgrading() {
   return IsUpgrading;
}

void IDaybreakEquipment::SetIsUpgrading(bool isUpgrading) {
    IsUpgrading = isUpgrading;
}

bool IDaybreakEquipment::IncreaseUpgradeProgress(float increase) {
    UpgradeProgress += increase;
	if (UpgradeProgress >= 1) {
		Upgrade();
		UpgradeProgress = 0;
		return true;
	}
	return false;
}

bool IDaybreakEquipment::DecreaseUpgradeProgress(float decrease) {
    UpgradeProgress -= decrease;
	if (UpgradeProgress <= 0) {
		UpgradeProgress = 0;
		return true;
	}
	return false;
}
