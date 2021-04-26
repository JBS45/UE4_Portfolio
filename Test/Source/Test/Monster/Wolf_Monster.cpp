// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf_Monster.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Player/BaseCharacter.h"
#include "MonsterAnimInstance.h"
#include "../Collision/WolfCollisionManager.h"
#include "MonsterArea.h"
#include "MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AWolf_Monster::AWolf_Monster() {
	Name = TEXT("Wolf");

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

	MoveCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MOVECOLLISION"));
	MoveCollision->SetupAttachment(GetCapsuleComponent());
	MoveCollision->SetCollisionProfileName(TEXT("Monster"));
	MoveCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	MoveCollision->bDynamicObstacle = true;

	HitBox = CreateDefaultSubobject<UWolfCollisionManager>(TEXT("HITBOX"));
	HitBox->SetUpAttachSocket(GetMesh());
	HitBox->SetUpCollisionType(FName(TEXT("MonsterHitBox")), ECollisionChannel::ECC_GameTraceChannel4);

/*	DamageBox = CreateDefaultSubobject<UWolfCollisionManager>(TEXT("DAMAGEBOX"));
	DamageBox->SetUpAttachSocket(GetMesh());
	DamageBox->SetUpCollisionType(FName(TEXT("MonsterDamage")), ECollisionChannel::ECC_GameTraceChannel6);
	*/
	//Todo : MonsterStatus Load DataTable;

	_Detect = CreateDefaultSubobject<UDetectComponent>(TEXT("DETECTMANAGER"));
	_Detect->SetDetectRange(DetectRange, EDetectCollisionType::E_PLAYER);
	_Detect->GetDetect()->SetupAttachment(GetCapsuleComponent());

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttackRange = 200;
	AggroChangeTerm = 15.0f;

	BasicSpeed = 200.0f;
	RunSpeed = 800.0f;
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	
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

void AWolf_Monster::SetUpMonsterStatus()
{
	CurrentHp = MonsterStatus.MaxHp;
	
}
void AWolf_Monster::SetArea(AMonsterArea* area) {
	ActiveArea = area;
}
FString AWolf_Monster::AWolf_Monster::GetMonsterName(){ return Name; }
AMonsterArea* AWolf_Monster::GetActiveArea() { return ActiveArea; }
void AWolf_Monster::SetActiveArea(AMonsterArea* area) { ActiveArea = area; }
void AWolf_Monster::SetPatrolNode(int max) {
	MaxPatrolNode = max;
	CurrentPatrolNode = 0;
}
void AWolf_Monster::NextPatrolNode() {
	CurrentPatrolNode++; 
	if (CurrentPatrolNode >= MaxPatrolNode) CurrentPatrolNode = 0;
}
int AWolf_Monster::GetCurrentPatrolNode() { return CurrentPatrolNode; }
bool AWolf_Monster::GetIsAlive() { return IsAlive; }
bool AWolf_Monster::GetIsFoundPlayer() { return IsFoundPlayer; }
void AWolf_Monster::SetIsFoundPlayer(bool value) { IsFoundPlayer = value; }
float AWolf_Monster::GetAggroChangeTerm() { return AggroChangeTerm; }
TArray<APawn*> AWolf_Monster::GetTargetList() { return _Detect->GetTargets(); }
float AWolf_Monster::GetAttackRange() { return AttackRange; }
void AWolf_Monster::SetTarget(ACharacter* target) { Target = target; }
EMonsterStateType AWolf_Monster::GetMonsterState() { return MonsterState; }
void AWolf_Monster::ChangeMonsterState(EMonsterStateType type) {
	if (MonsterState == type) return;
	MonsterState = type;

	switch (MonsterState)
	{
	case EMonsterStateType::E_NONE:
		break;
	case EMonsterStateType::E_IDLE:
		GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;
		break;
	case EMonsterStateType::E_FIND:
		break;
	case EMonsterStateType::E_BATTLE:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	case EMonsterStateType::E_HIT:
		break;
	case EMonsterStateType::E_DEAD:
		break;
	default:
		break;
	}
}

void AWolf_Monster::NotifyArea(ACharacter* target) {
	ActiveArea->Notify(target);
}