// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collision = CreateDefaultSubobject <UBoxComponent>(TEXT("DamaageBox"));

	RootComponent = Mesh;
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionObjectType(ECC_GameTraceChannel7);
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

}

void ABaseWeapon::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}
void ABaseWeapon::SetUpWeapon(EWeaponType type, UStaticMesh* mesh) {
	switch (type) {
		case EWeaponType::E_DUAL:
			Collision->SetBoxExtent(FVector(10, 10, 40));
			Collision->SetRelativeLocation(FVector(0, 0, 40));
			break;
		case EWeaponType::E_HAMMER:
			Collision->SetBoxExtent(FVector(20,32,80));
			Collision->SetRelativeLocation(FVector(0, 0, 40));
			break;
	}

	Mesh->SetStaticMesh(mesh);
	SetEnableWeapon(true);
	//Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnOverlapBegin);
	//Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnOverlapEnd);
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