// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Monster/BaseMonster.h"
#include "Player/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Collision/HitCollisionManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Collision = CreateDefaultSubobject <UBoxComponent>(TEXT("DamaageBox"));

	RootComponent = Mesh;
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionObjectType(ECC_GameTraceChannel7);
	Collision->SetCollisionProfileName("PlayerDamage");

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticle");
	TrailParticle->SetupAttachment(Mesh);


	static ConstructorHelpers::FObjectFinder<UParticleSystem>TRAIL_TEMPLATE(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_trail_thunder.P_ky_trail_thunder'"));
	UParticleSystem* TempTemplate = nullptr;
	if (TRAIL_TEMPLATE.Succeeded()) {
		TempTemplate = TRAIL_TEMPLATE.Object;
	}

	TrailParticle->Template = TempTemplate;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABaseWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//if I use do once function I can Implement Attack
	//Attack rate use Animation curve

	if (OtherActor->GetClass()->IsChildOf<ABaseMonster>()) {
		if (!DamagedMonster.Contains(OtherActor)) {
			HitCheck(OtherActor);
		}

	}
}

void ABaseWeapon::SetUpWeapon(EWeaponType type, USkeletalMesh* mesh,ACharacter* owner) {
	switch (type) {
		case EWeaponType::E_DUAL:
			CurrentBoxSize = FVector(10, 10, 40);
			CurrentBoxLocation = FVector(0, 0, 40);
			break;
		case EWeaponType::E_HAMMER:
			CurrentBoxSize = FVector(20, 32, 80);
			CurrentBoxLocation = FVector(0, 0, 40);
			break;
	}
	Collision->SetBoxExtent(CurrentBoxSize);
	Collision->SetRelativeLocation(CurrentBoxLocation);


	WeaponOwner = owner;
	Mesh->SetSkeletalMesh(mesh);
	SetEnableWeapon(true);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnOverlapBegin);

}
void ABaseWeapon::SetEnableWeapon(bool value) {

	Mesh->SetHiddenInGame(!value);
	if (value) {
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else {
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SetActorTickEnabled(value);
}
bool ABaseWeapon::HitCheck(AActor* Actor) {
	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);

	FVector Location = Collision->GetComponentLocation();
	FRotator Rotation = Collision->GetComponentRotation();

	GetWorld()->SweepMultiByChannel(
		DetectResult,
		Location,
		Location,
		Rotation.Quaternion(),
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeBox(CurrentBoxSize),
		params);

	DrawDebugBox(GetWorld(), Location, CurrentBoxSize, Rotation.Quaternion(), FColor::Blue, false, 1.0f);

	for (auto result :DetectResult) {
		if (result.Actor == Actor) {
			if (result.BoneName.ToString()!=FString("None")) {
				if (Cast<ABaseCharacter>(WeaponOwner)) {
					TESTLOG(Warning, TEXT("%f"), DamageRate);
					Cast<IDamageInterface>(WeaponOwner)->ApplyDamageFunc(result, DamageRate, EDamageType::E_NORMAL, NULL);
				}
				DamagedMonster.Add(Cast<ABaseMonster>(Actor));
				return true;
			}
		}
	}

	return false;
}

void ABaseWeapon::ResetDamagedMonster() {
	DamagedMonster.Empty();
}
void ABaseWeapon::SetOverlapEvent(bool IsOn) {
	Collision->SetGenerateOverlapEvents(IsOn);
	ResetDamagedMonster();
}

void ABaseWeapon::TrailOn(ETrailWidthMode type, float value) {
	TrailParticle->BeginTrails("TrailStart", "TrailCenter", type, value);
}
void ABaseWeapon::TrailOff() {
	TrailParticle->EndTrails();
}
void ABaseWeapon::SetDamageRate(float value) {
	DamageRate = value;
}