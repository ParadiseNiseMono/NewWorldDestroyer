// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthComponent.generated.h"

/**
 * Health bar component
 */
UCLASS()
class DESTROYER_API UHealthComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
