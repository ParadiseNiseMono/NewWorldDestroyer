// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Character/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthComponent;
class UPawnSensingComponent;

UCLASS()
class DESTROYER_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	void CheckPartrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	
protected:
	virtual void BeginPlay() override;

	// Play the hit reaction montage
	void PlayHitReactMontage(const FName& SectionName);

	void Die();

	void MoveToTarget(AActor* Target);

	AActor* ChoosePartrolTarget();

	UPROPERTY(BlueprintReadWrite)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	bool InTargetRange(AActor* Target, double Radius);
	UFUNCTION()
	void OnSeePawn(APawn* Target);
private:
	// Animation montages
	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Montages)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Particles)
	UParticleSystem* HitParticles;

	/*
	* Components
	*/
	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	UPawnSensingComponent* PawnSense;
	UPROPERTY(VisibleAnywhere, Category = AttributeComponents)
	UAttributeComponent* Attribute;

	UPROPERTY(VisibleAnywhere, Category = HealthComponents)
	UHealthComponent* HealthBarWidget;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 250.f;

	/*
	* navigation
	*/
	UPROPERTY()
	class AAIController* EnemyController;

	//Current Partrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
public:	


};
