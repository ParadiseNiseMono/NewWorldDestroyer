// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Treasures/Treasure.h"
#include "Character/DestroyerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADestroyerCharacter* Character = Cast<ADestroyerCharacter>(OtherActor);
	if (Character)
	{
		if(PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
		Destroy();
	}
}
