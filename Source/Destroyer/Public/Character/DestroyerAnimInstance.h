// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterTypes.h"
#include "DestroyerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DESTROYER_API UDestroyerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class ADestroyerCharacter* DestroyerCharacter;

	UPROPERTY(BlueprintReadOnly,category= Movement)
	class UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(BlueprintReadOnly, category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, category = "Movement|Character State")
	ECharacterState CharacterState;
};
