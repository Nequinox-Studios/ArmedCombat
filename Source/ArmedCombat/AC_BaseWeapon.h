// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AC_BaseWeapon.generated.h"

class AArmedCombatCharacter;
class UArmedGameplayAbility;

UCLASS()
class ARMEDCOMBAT_API AAC_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAC_BaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf< UArmedGameplayAbility>> WeaponAbilities;

	TArray<TSubclassOf< UArmedGameplayAbility>> GetWeaponAbilities();
};
