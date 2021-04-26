// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterArea.h"
#include "../BaseEnum.h"
#include "../Player/BaseCharacter.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMonsterArea::AMonsterArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsPlayerInRange = false;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DETECT"));

	DetectRange = 2000.0f;

	DetectSphere->SetCollisionObjectType(ECC_GameTraceChannel1);
	DetectSphere->SetSphereRadius(DetectRange);
	DetectSphere->SetCollisionProfileName(TEXT("DetectPlayer"));

	RootComponent = DetectSphere;
	
	Spawner = CreateDefaultSubobject<AMonsterSpawner>(TEXT("SPAWNER"));

}

// Called when the game starts or when spawned
void AMonsterArea::BeginPlay()
{
	Super::BeginPlay();
	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonsterArea::OnOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AMonsterArea::OnOverlapEnd);
	MonsterRegenTimer = MonsterRegenCoolTime;
}

// Called every frame
void AMonsterArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpawnUpdate(DeltaTime);
	
}
void AMonsterArea::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	auto target = Cast <ABaseCharacter>(OtherActor);
	TESTLOG(Warning, TEXT("InRange"));
	if (target != nullptr) {
		IsPlayerInRange = true;
	}
}
void AMonsterArea::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	auto target = Cast <ABaseCharacter>(OtherActor);
	TESTLOG(Warning, TEXT("OutRange"));
	if (target != nullptr) {
		IsPlayerInRange = false;
	}
}
void AMonsterArea::SpawnUpdate(float DeltaSeconds) {
	if (!IsPlayerInRange) {
		CurrentMonsterCount = Monsters.Num();
		MonsterRegenTimer += DeltaSeconds;

		if (CurrentMonsterCount < MaxMonsterCount &&
			MonsterRegenTimer >= MonsterRegenCoolTime) {

			Spawn();
			MonsterRegenTimer = 0.0f;
		}
	}
	
}
void AMonsterArea::Spawn() {
	if (SpawnMonsterType != nullptr) {
		ABaseMonster* result;
		result = Spawner->SpawnMonster(SpawnMonsterType, SpawnPoint->GetActorLocation(), SpawnRange);
		result->SetActiveArea(this);
		result->SetPatrolNode(PatrolPoint.Num());
		Monsters.Add(result);
	}
	CurrentMonsterCount = Monsters.Num();
}
void AMonsterArea::Reset() {
	for (auto monster : Monsters) {
		monster->SetIsFoundPlayer(false);
	}
}
void AMonsterArea::Notify(ACharacter* target) {
	for (auto monster : Monsters) {
		monster->SetIsFoundPlayer(true);
		monster->SetTarget(target);
		monster->ChangeMonsterState(EMonsterStateType::E_BATTLE);
	}
}