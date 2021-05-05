// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "DamageInterface.generated.h"
/**
 * 
 */
UINTERFACE(Blueprintable)

class TEST_API UDamageInterface : public UInterface
{
	GENERATED_BODY()

};
class TEST_API IDamageInterface 
{
	GENERATED_BODY()
public:
	virtual void ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0) PURE_VIRTUAL(UDamageInterface::ApplyDamageFunc);
	virtual void TakeDamageFunc(bool& OutIsWeak,int32& OutFinalDamage ,AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce=0)  PURE_VIRTUAL(UDamageInterface::TakeDamageFunc);
};
