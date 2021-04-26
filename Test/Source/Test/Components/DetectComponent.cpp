// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UDetectComponent::UDetectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	DetectRange = CreateDefaultSubobject<USphereComponent>("Detect");
	IsValid = false;

	
}


// Called when the game starts
void UDetectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDetectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDetectComponent::SetDetectRange(float range, EDetectCollisionType collison)
{
	SetOverlapType(collison);
	DetectRange->SetCollisionObjectType(ECC_GameTraceChannel1);
	DetectRange->OnComponentBeginOverlap.AddDynamic(this, &UDetectComponent::OnOverlapBegin);
	DetectRange->OnComponentEndOverlap.AddDynamic(this, &UDetectComponent::OnOverlapEnd);
	DetectRange->SetSphereRadius(range);
	Range = range;
	IsValid = true;
}

void UDetectComponent::DrawDebug(APawn* pawn, float deltaTime) {
	if (!IsValid) return;

#if ENABLE_DRAW_DEBUG
	//DrawDebugSphere(GetWorld(), pawn->GetActorLocation(), Range, 10, FColor::Red, false, 0.5f);
#endif
}

void UDetectComponent::SetOverlapType(EDetectCollisionType collison)
{
	switch (collison) {
	case EDetectCollisionType::E_PLAYER:
		DetectRange->SetCollisionProfileName(TEXT("DetectPlayer"));
		break;
	case EDetectCollisionType::E_MONSTER:
		DetectRange->SetCollisionProfileName(TEXT("DetectMonster"));
		break;
	default:
		break;
	}
}
void UDetectComponent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	auto target = Cast <APawn>( OtherActor);
	if (target!=nullptr) {
		Targets.Add(target);
		TESTLOG(Warning, TEXT("%d"), Targets.Num());
	}
}
void UDetectComponent::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	auto target = Cast <APawn>(OtherActor);
	if (target != nullptr) {
		if (Targets.Contains(target)) {
			Targets.Remove(target);
			TESTLOG(Warning, TEXT("%d"), Targets.Num());
		}
	}
}