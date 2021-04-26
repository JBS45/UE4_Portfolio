// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "GameFramework/Pawn.h"
#include "Components/ActorComponent.h"
#include "DetectComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetDetectRange(float range, EDetectCollisionType collison);
	void DrawDebug(APawn* pawn, float deltaTime);
	class USphereComponent* GetDetect() { return DetectRange; }
	TArray<APawn*> GetTargets() { return Targets; }

private:
	void SetOverlapType(EDetectCollisionType collison);
	UFUNCTION() 
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectRange;
	ECollisionChannel DetectType;
	float Range;
	bool IsValid;

	TArray<APawn*> Targets;
};
