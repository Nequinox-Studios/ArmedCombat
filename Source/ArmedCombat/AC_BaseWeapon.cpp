// Fill out your copyright notice in the Description page of Project Settings.

#include "AC_BaseWeapon.h"
#include "ArmedCombatCharacter.h"
#include "GAS/ArmedAbilitySystemComponent.h"
#include "GAS/ArmedGameplayAbility.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAC_BaseWeapon::AAC_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AAC_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAC_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<TSubclassOf< UArmedGameplayAbility>> AAC_BaseWeapon::GetWeaponAbilities()
{
	return WeaponAbilities;
}