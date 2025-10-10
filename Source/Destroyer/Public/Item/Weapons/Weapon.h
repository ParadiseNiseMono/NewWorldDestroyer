// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class DESTROYER_API AWeapon : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	AWeapon();
	void Equip(USceneComponent* SceneComponent,FName SocketName);
	void AttachWeaponToSocket(USceneComponent* SceneComponent, const FName& SocketName);
	TArray<AActor*> ActorsToIgnore;

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponCollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Trace Box")
	USceneComponent* TraceBoxStart;

	UPROPERTY(VisibleAnywhere, Category = "Trace Box")
	USceneComponent* TraceBoxEnd;



public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }
};
