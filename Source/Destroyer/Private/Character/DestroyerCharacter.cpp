// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DestroyerCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"	
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/Item.h"
#include "Item/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

// Sets default values
ADestroyerCharacter::ADestroyerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ADestroyerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
				Subsystem->AddMappingContext(DestroyerContext, 0);
		}
	}


}

// Called every frame
void ADestroyerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADestroyerCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState == EActionState::ECS_Attacking) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}
void ADestroyerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADestroyerCharacter::PickUp(const FInputActionValue& Value)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) {
		OverlappingWeapon->Equip(GetMesh(), FName("LeftHandSocket"));
		OverlappingItem = nullptr;
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
	}
}

void ADestroyerCharacter::Attack(const FInputActionValue& Value)
{
	if (CanAttack()) {
		ActionState = EActionState::ECS_Attacking;
		PlayAttackMontage();
	}
	
}

void ADestroyerCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Section = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Section)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

bool ADestroyerCharacter::CanAttack()
{
	return ActionState == EActionState::ECS_Unoccupied && CharacterState == ECharacterState::ECS_EquippedOneHanded;
}

void ADestroyerCharacter::AttackEnd()
{
	ActionState = EActionState::ECS_Unoccupied;
}

// Called to bind functionality to input
void ADestroyerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction,ETriggerEvent::Triggered,this,&ADestroyerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ADestroyerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(PickAction, ETriggerEvent::Triggered, this, &ADestroyerCharacter::PickUp);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADestroyerCharacter::Attack);
	}
}

