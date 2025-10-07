// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DestroyerAnimInstance.h"
#include "Character/DestroyerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDestroyerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	DestroyerCharacter = Cast<ADestroyerCharacter>(TryGetPawnOwner());
	if (DestroyerCharacter)
	{
		CharacterMovement = DestroyerCharacter->GetCharacterMovement();
	}
}

void UDestroyerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
		
	if (CharacterMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
		IsFalling = CharacterMovement->IsFalling();
		CharacterState = DestroyerCharacter->GetCharacterState();
	}
}
