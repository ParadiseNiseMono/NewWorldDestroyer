// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UCapsuleComponent;
UCLASS()
class DESTROYER_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UFUNCTION()
	void OnChaosBreak(const FChaosBreakEvent& BreakEvent);
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGeometryCollectionComponent* GeometryCollectionComponent;

	UPROPERTY(EditAnywhere, Category = "Treasure")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;


};
