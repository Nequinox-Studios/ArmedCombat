#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"


#include "GAS/ArmedAbilitySystemComponent.h"
#include "GAS/ArmedGameplayAbility.h"
#include "GAS/ArmAttributeSet.h"

#include "Knight.generated.h"

class AAC_BaseWeapon;


UCLASS()
class AKnight : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

		AKnight();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArmedAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UArmAttributeSet* Attributes;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AAC_BaseWeapon> StarterWeapon;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UArmedGameplayAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AAC_BaseWeapon* SpawnedWeapon = nullptr;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	void EquipStarterAbilities();

	virtual void InitializeAttributes();
	virtual void GiveAbilities();
};