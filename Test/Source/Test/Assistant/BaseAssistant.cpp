	// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAssistant.h"
#include "../Player/BaseCharacter.h"
#include "../Player/BasePlayerController.h"
#include "../Components/CharacterStatusManager.h"
#include "AssistantAIController.h"
#include "../Monster/BaseMonster.h"
#include "../BaseWeapon.h"
#include "AssistantInstance.h"


// Sets default values
ABaseAssistant::ABaseAssistant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Player")));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetCapsuleComponent()->bDynamicObstacle = true;


	GetCapsuleComponent()->SetCapsuleSize(34.0f, 60.0f);
	GetCapsuleComponent()->SetHiddenInGame(false);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -60.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector::OneVector *0.5f);


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MESH(TEXT("SkeletalMesh'/Game/Blueprints/Animations/Assistant/Mesh/castle_guard_01.castle_guard_01'"));

	if (SK_MESH.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MESH.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/Animations/Assistant/AssistantAnimBP.AssistantAnimBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>WEAPON_MESH(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_CrudeHammer/SK_Blunt_CrudeHammer.SK_Blunt_CrudeHammer'"));
	if (WEAPON_MESH.Succeeded()) {
		WeaponMesh = WEAPON_MESH.Object;
	}



	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	_StatusManager = CreateDefaultSubobject<UCharacterStatusManager>(TEXT("STATUSMANAGER"));
	_StatusManager->StatusInit();

	_Detect = CreateDefaultSubobject<UDetectComponent>(TEXT("DETECTMANAGER"));
	_Detect->SetDetectRange(DetectRange, EDetectCollisionType::E_MONSTER);
	_Detect->GetDetect()->SetupAttachment(GetCapsuleComponent());

	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->bUseRVOAvoidance = true;
	/*
	AIControllerClass = AAssistantAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	*/
	
	SetUpSkill();
}

// Called when the game starts or when spawned
void ABaseAssistant::BeginPlay()
{
	Super::BeginPlay();
	_AnimInst = Cast<UAssistantInstance>(GetMesh()->GetAnimInstance());

	Weapon = GetWorld()->SpawnActor<ABaseWeapon>();
	Weapon->SetActorScale3D(FVector::OneVector*0.6f);
	Weapon->SetEnableWeapon(true);
	Weapon->SetUpWeapon(EWeaponType::E_HAMMER, WeaponMesh,this);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Weapon")));


}

// Called every frame
void ABaseAssistant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SkillUpdate(DeltaTime);
	ActionCoolTimer(DeltaTime);
}

// Called to bind functionality to input
void ABaseAssistant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
float ABaseAssistant::PlayerDistance() {
	float result = FVector::Distance(GetActorLocation(), _Master->GetActorLocation());

	return result;
}

void ABaseAssistant::SetIsBattle(bool value) {
	IsBattle = value;
}
void ABaseAssistant::OnAttackMontageBegin() {
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("RightHand")));
	Weapon->SetActorRelativeLocation(FVector::ZeroVector);
}
void ABaseAssistant::OnAttackMontageEnd() {
	OnAttackEnd.Broadcast();
	GetCapsuleComponent()->SetWorldRotation(FQuat::Identity);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Weapon")));
	Weapon->SetActorRelativeLocation(FVector::ZeroVector);
}
bool ABaseAssistant::BaseAttack() {
	if (UseSkill(TEXT("ComboAttack"))) {
		return true;
	}
	if (UseSkill(TEXT("SwingAttack"))) {
		return true;
	}
	if (UseSkill(TEXT("Attack"))){
		return true;
	}

	CanAction = false;
	return false;
}

void ABaseAssistant::SetUpSkill() {
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/Assistant/AssistantSkill.AssistantSkill'"));

	if (DataTable.Succeeded()) {
		auto TEMPDATA = DataTable.Object;
		auto RowNames = TEMPDATA->GetRowNames();
		static const FString ContextString(TEXT("Set Up Assistant Skill"));
		for (auto name : RowNames) {
			auto rowData = TEMPDATA->FindRow<FAssistSkill>(name,ContextString,true);
			Skills.Add(rowData->Name, *rowData);
		}
	}

}
void ABaseAssistant::SkillUpdate(float Deltaseconds) {
	for (auto skill : Skills) {
		if (!Skills[skill.Key].IsReady) {
			Skills[skill.Key].CurrentLeftTime -= Deltaseconds;
			if (Skills[skill.Key].CurrentLeftTime <= 0) {
				Skills[skill.Key].IsReady = true;
				Skills[skill.Key].CurrentLeftTime = 0.0f;
				CanAction = true;
			}
		}
	}
}
bool ABaseAssistant::UseSkill(FString skillName) {
	if (Skills[skillName].IsReady) {
		_AnimInst->PlayAnimMontage(Skills[skillName].Animation);
		Skills[skillName].CurrentLeftTime = Skills[skillName].CoolTime;
		Skills[skillName].IsReady = false;
		return true;
	}
	return false;
}
void ABaseAssistant::ActionCoolTimer(float Deltaseconds) {
	if (!CanAction) {
		ActionTimer += Deltaseconds;
		if (ActionTimer >= ActionCoolTime) {
			ActionTimer = 0.0f;
			CanAction = true;
		}
	}
}