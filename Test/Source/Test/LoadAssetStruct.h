// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Test.h"
#include "Engine/DataTable.h"
#include "WeaponAnimTable.h"
#include "LoadAssetStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLoadAssetTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString reference;

};