// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../WeaponAnimTable.h"
#include "Engine/DataTable.h"
#include "../BaseEnum.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "../Player/BaseCharAnimInstance.h"
#include "InputBufferManager.generated.h"

#define COMMAND_BASE_STATE "Base"
#define COMMAND_DRAWWEAPON "Draw"
#define COMMAND_PUTUPWEAPON "PutUp"
#define COMMAND_EVADE "Evade"
#define INPUT_BUFFER_DELAY 0.05f


USTRUCT(BlueprintType)
struct FInputInfo {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		EInputKey InputType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float  TimeStamp;
	bool operator==(const FInputInfo& other) const;
};

USTRUCT(BlueprintType)
struct FCommand {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		TArray<EInputKey> Inputs;
};
USTRUCT(BlueprintType)
struct FChainAction {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString AttackName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Priority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCommand> Commands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> EnableChainAction;
};
USTRUCT(BlueprintType)
struct FChainActionTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FChainAction Action;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UInputBufferManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputBufferManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		TArray<FInputInfo> InputBuffer;

	float removeInputFromBufferTime;

	FVoidDelegate EvadeDelegate;
	FVoidDelegate CommandUpdateDelegate;
private:
	UPROPERTY(VisibleAnywhere, Category = "State")
	EWeaponType CurrentWeapon;
	UPROPERTY(VisibleAnywhere, Category = "State")
	ECharacterState CurrentState;
	UPROPERTY(VisibleAnywhere, Category = "State")
	FString CurrentCommandName;
	UPROPERTY(VisibleAnywhere, Category = "State")
	UBaseCharAnimInstance* TargetAnimInst;
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool CanAcitonInput;

	TMap<EWeaponType, TMap<FString, FChainAction>> TotalCommands;
	TMap<FString, FChainAction>* CurrentCommands;
	TArray<FString>* EnableAction;

public:
	UFUNCTION(BlueprintCallable)
		void AddInputToInputBuffer(FInputInfo input);
	UFUNCTION()
		void CheckInputBufferCommand();

	void CheckBufferTime();
	void ChangeCommandDT(EWeaponType weapon);
	UAnimMontage* ActiveCommand(FString NextCommand);
	void SetTargetAnimInst(UBaseCharAnimInstance* anim) { TargetAnimInst = anim; }
	void SetCharacterState(ECharacterState type) { CurrentState = type; }

	void ChainReset();
	void PutUpWeapon();
	inline void InputBufferOpen() { CanAcitonInput = true; }
	inline void InputBufferClose() { CanAcitonInput = false; }
	TArray<FString> GetEnableAction() { return *EnableAction; }
	TArray<EInputKey> GetCommandFromName(FString commandName) { return ((CurrentCommands->Find(commandName)->Commands[0]).Inputs); }

private:
	bool BufferCheckDelay();
};
