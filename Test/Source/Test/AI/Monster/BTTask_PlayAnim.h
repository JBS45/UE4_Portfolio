// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayAnim.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTask_PlayAnim : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayAnim();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
};

UCLASS()
class TEST_API UBTTask_PlayAnimToForward : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayAnimToForward();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
	UPROPERTY(EditAnywhere, Category = "Distance", meta = (AllowPrivateAccess = "true"))
		float distance;
};

UCLASS()
class TEST_API UBTTask_PlayTwoAnim : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayTwoAnim();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float Degree;
	float Lerpspeed;
	FRotator DestRotation;
};
