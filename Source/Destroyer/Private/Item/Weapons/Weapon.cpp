// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Weapons/Weapon.h"
#include "Character/DestroyerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"


AWeapon::AWeapon()
{
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(RootComponent);
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	TraceBoxStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceBoxStart"));
	TraceBoxStart->SetupAttachment(RootComponent);
	TraceBoxEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceBoxEnd"));
	TraceBoxEnd->SetupAttachment(RootComponent);
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}
void AWeapon::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Start = TraceBoxStart->GetComponentLocation();
	FVector End = TraceBoxEnd->GetComponentLocation();

	TArray<AActor*> IgnoreActors;	
	IgnoreActors.Add(this);
	for(AActor* Actor : ActorsToIgnore)
	{
		IgnoreActors.AddUnique(Actor);
	}
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f,5.f,5.f),
		TraceBoxStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	if (BoxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface) {
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		} 
		ActorsToIgnore.Add(BoxHit.GetActor());
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::Equip(USceneComponent* SceneComponent, FName SocketName)
{
	AttachWeaponToSocket(SceneComponent, SocketName);
	ItemState = EItemState::EIS_Equipped;
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, EquipSound, GetActorLocation()
		);
	}
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachWeaponToSocket(USceneComponent* SceneComponent, const FName& SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}
