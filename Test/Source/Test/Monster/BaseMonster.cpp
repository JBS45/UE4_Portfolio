// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "MonsterAIController.h"
#include "../Player/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "../Collision/HitCollisionManager.h"
#include "../Components/MonsterStatusManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(MonsterTag);
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionProfileName("MonsterHitBox");
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ABaseMonster::OnOverlapBegin);
	TESTLOG(Warning, TEXT("Monster Begin"));
}

// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ABaseMonster::SetBrokenState(EMonsterPartsType brokenPart) {
	EMonsterBrokenParts part = EMonsterBrokenParts::E_NONE;
	switch (brokenPart)
	{
	case EMonsterPartsType::E_HEAD:
		part = EMonsterBrokenParts::E_HEAD;
		break;
	case EMonsterPartsType::E_BODY:
		part = EMonsterBrokenParts::E_BODY;
		break;
	case EMonsterPartsType::E_LEFTHAND:
		part = EMonsterBrokenParts::E_LEFTHAND;
		break;
	case EMonsterPartsType::E_RIGHTHAND:
		part = EMonsterBrokenParts::E_RIGHTHAND;
		break;
	case EMonsterPartsType::E_WING:
		part = EMonsterBrokenParts::E_WING;
		break;
	case EMonsterPartsType::E_LEFTLEG:
		part = EMonsterBrokenParts::E_LEFTLEG;
		break;
	case EMonsterPartsType::E_RIGHTLEG:
		part = EMonsterBrokenParts::E_RIGHTLEG;
		break;
	case EMonsterPartsType::E_TAIL:
		part = EMonsterBrokenParts::E_TAIL;
		break;
	}
	BrokenState |= (int32)part;
}

ABaseCharacter* ABaseMonster::GetTarget() {
	ABaseCharacter* result = nullptr;
	auto targets = _Detect->GetTargets();
	for (int i = 0; i < targets.Num(); ++i) {
		if (Cast<ABaseCharacter>(targets[i])) {
			result = Cast<ABaseCharacter>(targets[i]);
			break;
		}
	}
	return result;
}
void ABaseMonster::GetDamageCapsules() {
	auto TempArray = GetComponents();
	TArray<UCapsuleComponent*> Capsules;
	for (auto element : TempArray) {
		if (Cast<UCapsuleComponent>(element) && element->ComponentTags.Num()>0) {
			Capsules.Add(Cast<UCapsuleComponent>(element));
			Cast<UCapsuleComponent>(element)->SetCollisionProfileName("MonsterDamage");
			Cast<UCapsuleComponent>(element)->SetGenerateOverlapEvents(true);
		}
	}
	SetDamageBox(Capsules);
}
void ABaseMonster::SetDamageBox(TArray<class UCapsuleComponent*> arr) {
	for (auto element : arr) {

		auto Type = element->ComponentTags[0];

		if (Type.ToString() == "Head") {
			//TESTLOG(Warning, TEXT("Head"));
			DamageBox.Add(EMonsterPartsType::E_HEAD, element);
		}
		else if (Type.ToString() == "LeftHand") {
			//TESTLOG(Warning, TEXT("LeftHand"));
			DamageBox.Add(EMonsterPartsType::E_LEFTHAND, element);
		}
		else if (Type.ToString() == "RightHand") {
			//TESTLOG(Warning, TEXT("RightHand"));
			DamageBox.Add(EMonsterPartsType::E_RIGHTHAND, element);
		}
		else if(Type.ToString() == "Tail"){
			//TESTLOG(Warning, TEXT("Tail"));
			DamageBox.Add(EMonsterPartsType::E_TAIL, element);
		}
		else {
			//TESTLOG(Warning, TEXT("Body"));
			DamageBox.Add(EMonsterPartsType::E_BODY, element);
		}
	}
}
void ABaseMonster::SetUpAnim(const FString DataRef) {
	static ConstructorHelpers::FObjectFinder<UDataTable> DATATABLE(*DataRef);
	if (DATATABLE.Succeeded()) {
		auto DataTable = DATATABLE.Object;
		auto Names = DataTable->GetRowNames();
		for (auto name : Names) {
			auto row = DataTable->FindRow<FMonsterAnim>(name, FString(TEXT("Init Monster data")));
			AnimationMap.Add(row->Name, row->Animation);
		}
	}
}

float ABaseMonster::GetAggroChangeTerm() const {
	return AggroChangeTerm; 
}
float ABaseMonster::GetAttackRange() const {
	return AttackRange;
}
void ABaseMonster::SetActiveArea(class AMonsterArea* area) { 
	ActiveArea = area; 
}
AMonsterArea* ABaseMonster::GetActiveArea() {
	return ActiveArea; 
}
UHitCollisionManager* ABaseMonster::GetCollisionManager() {
	return HitBox; 
}
FString ABaseMonster::GetMonsterName()const {
	return MonsterName; 
}
uint8 ABaseMonster::GetMonsterID() const {
	return MonsterID; 
}
TArray<APawn*> ABaseMonster::GetTargetList() { 
	return _Detect->GetTargets(); 
}
EMonsterState ABaseMonster::GetMonsterState() { 
	return CurrentState; 
}
int32 ABaseMonster::GetBrokenState() const {
	return BrokenState; 
}
AMonsterAIController* ABaseMonster::GetMonsterController() { 
	return MonsterController; 
}
UMonsterAnimInstance* ABaseMonster::GetAnimInst() {
	return AnimInst; 
}
float ABaseMonster::GetRoarRange() { 
	return RoarRange; 
}

void ABaseMonster::ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType, const float ImpactForce ) {
	auto TargetActor = hit.Actor;

	bool IsWeak = false;

	float Damage = Status->GetDamage();
	int32 CaculateDamage = 0;

	Damage *= AcitonDamageRate;
	Cast<IDamageInterface>(TargetActor)->TakeDamageFunc(IsWeak, CaculateDamage, this, hit, Damage, DamageType, ImpactForce);
}
void ABaseMonster::TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType , const float ImpactForce) {
	HitBox->ReceiveDamage(hit,hit.BoneName, CaculateDamage, OutFinalDamage, OutIsWeak);
	if (OutFinalDamage == 0) {
		TESTLOG(Warning,TEXT("Bone : %s, Component : %s"),*(hit.BoneName.ToString()),*(hit.Component->GetName()))
	}
	Status->TakeDamage(OutFinalDamage);

	if (CurrentState == EMonsterState::E_IDLE
		&& Target == nullptr) {
		Target = Cast<ABaseCharacter>(Causer);
		MonsterController->SetTarget(Target);
		MonsterController->ChangeMonsterState(EMonsterState::E_BATTLE);
	}

}

UAnimMontage* ABaseMonster::GetMontage(FString name) const {
	UAnimMontage* result= AnimationMap[name];
	return result;
}
void ABaseMonster::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//if I use do once function I can Implement Attack
	//Attack rate use Animation curve
	if (IsAttack&&OtherActor->GetClass()->IsChildOf<ABaseCharacter>()) {
		if (!DamagedCharacter.Contains(OtherActor)) {
			
			auto Bones = HitBox->GetHitBox()[AttackEnablePart];
			auto Names = Bones->GetBones();
			for (auto name : Names) {
				HitCheck(name);
			}
		}
	}
}

void ABaseMonster::HitCheck(FName Bone) {

	float Radius = 120.0f;
	FVector Location = GetMesh()->GetSocketLocation(Bone);
	FQuat Rotation = GetMesh()->GetSocketQuaternion(Bone);

	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);
	bool Result = GetWorld()->SweepMultiByProfile(
		DetectResult,
		Location,
		Location,
		Rotation,
		TEXT("MonsterDamage"),
		FCollisionShape::MakeSphere(Radius),
		params);

	DrawDebugSphere(GetWorld(), Location, Radius, 6, FColor::Red, false, 1.0f);



	for (auto result : DetectResult) {
		auto Player = Cast<ABaseCharacter>(result.Actor);
		if (IsValid(Player)&& (!DamagedCharacter.Contains(Player)))
		{
			ApplyDamageFunc(result, DamageRate, AttackDamageType, KnockBackForce);
			DamagedCharacter.Add(Player);
		}

	}
}

void ABaseMonster::ResetDamagedPlayer() {
	DamagedCharacter.Empty();
}

void ABaseMonster::PartOverlapOn(EMonsterPartsType Parts) {
	IsAttack = true;
	
	if (Parts == EMonsterPartsType::E_ALLBODY) {
		for (auto box : DamageBox) {
			box.Value->SetGenerateOverlapEvents(true);
		}
	}
	else {
		TArray<UCapsuleComponent*> Boxes;
		DamageBox.MultiFind(Parts, Boxes);
		for (auto box : Boxes){
			box->SetGenerateOverlapEvents(true);
		}
	}
	
}
void ABaseMonster::PartOverlapOff() {
	for (auto box : DamageBox) {
		box.Value->SetGenerateOverlapEvents(false);
	}
	ResetDamagedPlayer();
}
TArray<UCapsuleComponent*> ABaseMonster::GetDamageBox(EMonsterPartsType Parts) {
	TArray<UCapsuleComponent*> result;
	DamageBox.MultiFind(Parts, result);
	return result;
}
void ABaseMonster::SetMeleeDamage(float damageRate, float force, EDamageType type, EMonsterPartsType EnablePart) {
	DamageRate = damageRate;
	KnockBackForce = force;
	AttackDamageType = type;
	AttackEnablePart = EnablePart;
}