// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonFireBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../Components/DamageInterface.h"
#include "DrawDebugHelpers.h"


// Sets default values
ADragonFireBall::ADragonFireBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	HitBox = CreateDefaultSubobject<USphereComponent>("HitBox");
	HitBox->InitSphereRadius(HitBoxRange);
	HitBox->SetCollisionProfileName("MonsterProjectile");
	RootComponent = HitBox;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->SetUpdatedComponent(HitBox);
	Movement->InitialSpeed = ProjectileSpeed;
	Movement->MaxSpeed = ProjectileSpeed;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 0.0f;

	ProjectileParticle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	ProjectileParticle->SetupAttachment(HitBox);
	ProjectileParticle->SetWorldScale3D(FVector::OneVector * 1.5f);
	ProjectileParticle->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PROJECTILE_MESH(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_shot_fire.P_ky_shot_fire'"));
	if (PROJECTILE_MESH.Succeeded()) {
		BaseTemplate = PROJECTILE_MESH.Object;
	}
	ProjectileParticle->Template = BaseTemplate;

	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>("Explosion");
	static ConstructorHelpers::FObjectFinder<UParticleSystem>EXPLOSION_MESH(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_fire.P_ky_hit_fire'"));
	if (EXPLOSION_MESH.Succeeded()) {
		ExplosionTemplate = EXPLOSION_MESH.Object;
	}
	ExplosionParticle->SetupAttachment(HitBox);
	ExplosionParticle->Template = ExplosionTemplate;
	ExplosionParticle->SetWorldScale3D(FVector::OneVector * 2.5f);
	ExplosionParticle->bAutoActivate = false;


	HitBox->OnComponentHit.AddDynamic(this, &ADragonFireBall::OnHit);
}

// Called when the game starts or when spawned
void ADragonFireBall::BeginPlay()
{
	Super::BeginPlay();
	TESTLOG(Warning, TEXT("%s"), *(GetActorLocation().ToString()));
}


void ADragonFireBall::SpawnFireBall(float damage, float explosion, AActor* causer, EDamageType type, float force)
{
	Damage = damage;
	ProjectileCauser = causer;
	ExplosionRange = explosion;
	ProjectileDamageType = type;
	ProjectileForce = force;

	IsWorking = true;
	ProjectileParticle->SetHiddenInGame(false);
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Movement->SetUpdatedComponent(HitBox);
}
void ADragonFireBall::ShootDirection(const FVector& ShootDirection)
{
	Movement->Velocity = ShootDirection * Movement->InitialSpeed;
}

void ADragonFireBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	ProjectileParticle->SetHiddenInGame(true);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Explode(Hit);

	ExplosionParticle->SetHiddenInGame(false);
	ExplosionParticle->Activate();
}
void ADragonFireBall::Explode(const FHitResult& Hit) {
	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);
	auto ExplodePoint = Hit.ImpactPoint;

	bool bHit = GetWorld()->SweepMultiByProfile(
		DetectResult,
		ExplodePoint,
		ExplodePoint,
		FQuat::Identity,
		TEXT("MonsterDamage"),
		FCollisionShape::MakeSphere(ExplosionRange),
		params);

	DrawDebugSphere(GetWorld(), ExplodePoint, ExplosionRange, 30, FColor::Green, false, 0.5f);

	//Todo : ControllingPawn Play Sound Roar

	bool IsWeak = false;
	int32 OutFinalDamage = 0;

	TArray<AActor*> repeatedActor;

	for (auto DetectCharacter : DetectResult) {

		auto Character = Cast<IDamageInterface>(DetectCharacter.Actor);

		if (Character!=nullptr && !repeatedActor.Contains(DetectCharacter.GetActor())) {
			TESTLOG(Warning, TEXT("%s"), *(DetectCharacter.Actor->GetName()));
			Character->TakeDamageFunc(IsWeak, OutFinalDamage, this, DetectCharacter, DamageRate*Damage, ProjectileDamageType, NULL);
			repeatedActor.Add(DetectCharacter.GetActor());
		}
	}
	GetWorldTimerManager().SetTimer(CheckTimer, this, &ADragonFireBall::EnableMode, 1.0f, true, 1.0f);
	

}
void ADragonFireBall::EnableMode() {
	if (ExplosionParticle->HasCompleted()) {
		TESTLOG(Warning, TEXT("Not Working"));
		IsWorking = false;
		ProjectileParticle->SetHiddenInGame(true);
		ExplosionParticle->SetHiddenInGame(true);
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PrimaryActorTick.bCanEverTick = false;
		GetWorldTimerManager().ClearTimer(CheckTimer);
	}
}

bool ADragonFireBall::GetIsWorking() const {
	return IsWorking;
}