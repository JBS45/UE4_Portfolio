// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AssistantInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UAssistantInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAssistantInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float CharSpeed;
public:
	void PlayAnimMontage(UAnimMontage* montage);
};
