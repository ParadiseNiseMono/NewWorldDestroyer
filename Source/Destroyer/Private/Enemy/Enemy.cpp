// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Destroyer/DebugMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Component/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));
	HealthBarWidget = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget) {
		HealthBarWidget->SetHealthPercent(Attribute->GetHealth() / Attribute->GetMaxHealth());
		HealthBarWidget->SetVisibility(false);
	}
	EnemyController= Cast<AAIController>(GetController());
	if (EnemyController && PartrolTarget) {
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PartrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		for (auto& point : PathPoints) {
			const FVector& Location = point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12.f, FColor::Blue, true);
		}

	}
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


void AEnemy::Die()
{
	//play death montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		const int32 Section = FMath::RandRange(0, 4);
		FName SectionName = FName();
		switch (Section)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		default:
			break;
		}
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget) {
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
		if (DistanceToTarget > CombatRadius) {
			CombatTarget = nullptr;
			if (HealthBarWidget) {
				HealthBarWidget->SetVisibility(false);
			}
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint )
{
	//DRAW_SPHERE_CUSTOM(ImpactPoint, 8.f, 12, FColor::Red, false, 5.f);
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);
	}
	if (Attribute && Attribute->IsAlive()) {
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	

	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
	if (HitParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	if (Theta <= 45.f)
	{
		PlayHitReactMontage(FName("FromFront"));
	}
	else if (Theta > 45.f && Theta <= 135.f)
	{
		const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
		if (CrossProduct.Z >0)// right side
		{
			PlayHitReactMontage(FName("FromRight"));
		}
		else
		{
			PlayHitReactMontage(FName("FromLeft"));
		}
	}
	else if (Theta > 135.f)
	{
		PlayHitReactMontage(FName("FromBack"));
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attribute) {
		Attribute->RecieveDamage(DamageAmount);
		if (HealthBarWidget) {
			HealthBarWidget->SetHealthPercent(Attribute->GetHealth()/Attribute->GetMaxHealth());
		}
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

