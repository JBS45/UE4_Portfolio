// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf_Monster.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Player/BaseCharacter.h"
#include "MonsterAnimInstance.h"
#include "MonsterArea.h"
#include "MonsterAIController.h"
#include "../Components/MonsterStatusManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AWolf_Monster::AWolf_Monster() {
	MonsterID = 101;

	//Root = CreateDefaultSubobject<UBoxComponent>("Base");


	//Root->SetBoxExtent(FVector(112.0f, 64.0f, 64.0f));
	GetMesh()->SetRelativeTransform(FTransform(FRotator(0, -90, 0).Quaternion(), FVector(0, 0, -80.0f), FVector::OneVector));

	//Root->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Root->SetCollisionProfileName(TEXT("Monster"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BASEMESH(TEXT("SkeletalMesh'/Game/QuadrapedCreatures/Barghest/Meshes/SK_BARGHEST.SK_BARGHEST'"));
	if (BASEMESH.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BASEMESH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/C++/Monster/WolfAnimBP.WolfAnimBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}


	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;

	//Todo : MonsterStatus Load DataTable;

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttackRange = 200;

	BasicSpeed = 200.0f;
	RunSpeed = 800.0f;
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	
	CurrentState = EMonsterState::E_CREATE;
}

void AWolf_Monster::BeginPlay()
{
	Super::BeginPlay();
	AnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());

}

void AWolf_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWolf_Monster::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AWolf_Monster::NotifyArea(ACharacter* target) {
	ActiveArea->Notify(target);
}

bool AWolf_Monster::MonsterInit(FMonsterStatus status) {

	Status->DeadDel.AddUObject(this, &AWolf_Monster::Dead);
	CurrentState = EMonsterState::E_IDLE;

	return false;
}
void AWolf_Monster::Dead() {
	CurrentState = EMonsterState::E_DEAD;
}
void AWolf_Monster::ChangeMonsterState(EMonsterState state) {

}

bool AWolf_Monster::TakeDamageFromPlayer(const FHitResult& hit, const FName socketName, float dmg, int32& outDmage, bool& IsWeak) {
	return false;
}