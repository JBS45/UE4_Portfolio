// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusManager.h"

// Sets default values
UCharacterStatusManager::UCharacterStatusManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void UCharacterStatusManager::BeginPlay()
{
	Super::BeginPlay();
	IsAlive = true;
	IsSprint = false;
}

// Called every frame
void UCharacterStatusManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsAlive) {
		ReGen(DeltaTime);
	}
	if (IsSprint) {
		SprintUseStamina(DeltaTime);
	}
	else {
		RegenStamina(DeltaTime);
	}

}
void UCharacterStatusManager::StatusInit() {
	CreateCharState();

	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEquipmentState"), true);
	if (enumPtr != nullptr) {
		for (int i = 0; i < enumPtr->NumEnums(); ++i) {
			EEquipmentState Key = (EEquipmentState)enumPtr->GetValueByIndex(i);
			FEquipmentStatus Value;
			CharacterStatus.Slot.Add(Key, Value);
		}
	}

	UpdateStatus();

	CharacterStatus.HpRegen = 2.0f;
	CharacterStatus.StaminaRegen = 15.0f;
	CharacterStatus.CurrentHp = CharacterStatus.MaxHp;
	CharacterStatus.CurrentStamina = CharacterStatus.MaxStamina;

}
void UCharacterStatusManager::CreateCharState() {
	CharacterStatus.CharacterStatus.Hp = 100.0f;
	CharacterStatus.CharacterStatus.Stamina = 100.0f;
	CharacterStatus.CharacterStatus.Dmg = 10.0f;
	CharacterStatus.CharacterStatus.Def = 0.0f;
	CharacterStatus.CharacterStatus.CriticalRate = 0.0f;

}
void UCharacterStatusManager::EquipItem(const FEquipmentStatus item) {
	
	auto TargetSlot = CharacterStatus.Slot[item.EquipType];

	if (TargetSlot.IsEmpty) {
		TargetSlot = item;
		TargetSlot.IsEmpty = false;
	}
	else {
		//TODO : EXCHANGE ITEM DATA
	}

	UpdateStatus();
}
void UCharacterStatusManager::UpdateStatus() {
	FBaseStatus TempStatus = CharacterStatus.CharacterStatus;

	for (auto slotdata : CharacterStatus.Slot) {
		if (!slotdata.Value.IsEmpty) {
			TempStatus += slotdata.Value.StatusData;
		}
	}
	
	CharacterStatus.MaxHp = TempStatus.Hp;
	CharacterStatus.MaxStamina = TempStatus.Stamina;
	CharacterStatus.Dmg = TempStatus.Dmg;
	CharacterStatus.Def = TempStatus.Def;
	CharacterStatus.CriticalRate = TempStatus.CriticalRate;

	OnStatusUpdate.ExecuteIfBound();
}

void UCharacterStatusManager::ReGen(const float deltaTime) {
	if (CharacterStatus.CurrentHp < CharacterStatus.MaxHp) {
		CharacterStatus.CurrentHp += CharacterStatus.HpRegen*(deltaTime / ReGenBaseTime);
	}
	else {
		CharacterStatus.CurrentHp = CharacterStatus.MaxHp;
	}
	OnStatusUpdate.ExecuteIfBound();
}
bool UCharacterStatusManager::UseStamina() {
	if (CharacterStatus.CurrentStamina >= CharacterStatus.EvadeUseStaminaAmount) {
		CharacterStatus.CurrentStamina -= CharacterStatus.EvadeUseStaminaAmount;

		OnStatusUpdate.ExecuteIfBound();
		return true;
	}
	return false;
}


float UCharacterStatusManager::GetHpRate() {
	return CharacterStatus.CurrentHp / CharacterStatus.MaxHp; 
}
float UCharacterStatusManager::GetStaminaRate() {
	return CharacterStatus.CurrentStamina / CharacterStatus.MaxStamina; 
}
float UCharacterStatusManager::GetDamage() {
	return CharacterStatus.Dmg; 
}
float UCharacterStatusManager::GetCritical() {
	return CharacterStatus.CriticalRate; 
}
void UCharacterStatusManager::SetIsSprint(bool value) {
	IsSprint = value; 
}
int32 UCharacterStatusManager::TakeDamage(float Damage) {
	int32 FinalDamage = 0;
	if (CharacterStatus.Def <= 0) {
		FinalDamage = FMath::FloorToInt(Damage);
	}
	else {
		float DefRate = CharacterStatus.Def / (CharacterStatus.Def + 100);
		FinalDamage = FMath::FloorToInt(Damage*DefRate);
	}
	CharacterStatus.CurrentHp -= FinalDamage;

	if (CharacterStatus.CurrentHp <= 0) {
		OnCharacterDeadDel.Execute();
	}

	return FinalDamage;
}


void UCharacterStatusManager::RegenStamina(float delta) {
	if (CharacterStatus.CurrentStamina < CharacterStatus.MaxStamina) {
		CharacterStatus.CurrentStamina += CharacterStatus.StaminaRegen*(delta / ReGenBaseTime);
	}
	else {
		CharacterStatus.CurrentStamina = CharacterStatus.MaxStamina;
	}
}
void UCharacterStatusManager::SprintUseStamina(float delta) {
	if (CharacterStatus.CurrentStamina > 0) {
		CharacterStatus.CurrentStamina -= CharacterStatus.StaminaConsumeSprint* (delta / ReGenBaseTime);
	}
	else {
		CharacterStatus.CurrentStamina = 0;
		OnStaminaZeroDel.Execute();
	}
}

const FBaseStatus FBaseStatus::operator+(const FBaseStatus& rhs) {
	return FBaseStatus(*this) += rhs;
}

FBaseStatus& FBaseStatus::operator+=(const FBaseStatus& rhs) {
	this->Hp += rhs.Hp;
	this->Stamina += rhs.Stamina;
	this->Dmg += rhs.Dmg;
	this->Def += rhs.Def;
	this->CriticalRate += rhs.CriticalRate;
	return *this;
}
