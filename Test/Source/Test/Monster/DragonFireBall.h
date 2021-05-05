// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "GameFramework/Actor.h"
#include "DragonFireBall.generated.h"

UCLASS()
class TEST_API ADragonFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADragonFireBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* HitBox;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		float HitBoxRange = 150.0f;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		class UParticleSystemComponent* ProjectileParticle;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UParticleSystem* BaseTemplate;

	UPROPERTY(VisibleAnywhere, Category = Explosion)
		class UParticleSystemComponent* ExplosionParticle;
	UPROPERTY(VisibleAnywhere, Category = Explosion)
		UParticleSystem* ExplosionTemplate;
	UPROPERTY(VisibleAnywhere, Category = Explosion)
		float DamageRate = 2.0f;


	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* Movement;
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float ProjectileSpeed = 3000.0f;
	
	bool IsWorking = false;
	float Damage = 0.0f;
	float ExplosionRange = 200.0f;
	float ProjectileForce;
	AActor* ProjectileCauser;
	EDamageType ProjectileDamageType;
	FTimerHandle CheckTimer;

public:
	FORCEINLINE bool GetIsWorking() const;
	void ShootDirection(const FVector& ShootDirection);
	void SpawnFireBall(float damage, float explosion, AActor* causer, EDamageType type = EDamageType::E_KNOCKBACK,float force = 0);
private:
	void EnableMode();
	void Explode(const FHitResult& Hit);
private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
