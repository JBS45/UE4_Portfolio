// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "Components/ActorComponent.h"
#include "CommandTableManager.generated.h"


class UAnimMontage;
DECLARE_DELEGATE_OneParam(FVoidDelegateChain, TArray<FChainAction>);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UComandTableManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComandTableManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	FVoidDelegateChain NewActionChainDel;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		EWeaponType CurrentWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		FString CurrentCommandName;
private:
	TMap<EWeaponType, TMap<FString, FChainAction>> TotalCommands;
	TMap<FString, FChainAction>* CurrentCommands;
	TArray<FString> EnableAction;
private:
	void LoadCommandTable(FString ref);
	UAnimMontage* FindAnmation(FString actionName);

public:
	FORCEINLINE bool ValidCurrentCommandName() const;
	FORCEINLINE FChainAction FindAction(FString actionName);
	void SetCurrentCommandName(FString commandName);
	TArray<FString> FindEnableAction(FString currentActionName);
	void ChangeCommandTable(EWeaponType weapon);

};
