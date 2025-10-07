#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOneHanded UMETA(DisplayName = "Equipped OneHand"),
	ECS_EquippedTwoHanded UMETA(DisplayName = "Equipped TwoHand"),
};
UENUM(BlueprintType)
enum class EActionState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Attacking UMETA(DisplayName = "Attacking")

};