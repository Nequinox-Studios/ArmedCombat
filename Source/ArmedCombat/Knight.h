#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "Knight.generated.h"

class AAC_BaseWeapon;
class UArmedAbilitySystemComponent;

UCLASS()
class AKnight : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArmedAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UArmAttributeSet* Attributes;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AAC_BaseWeapon> StarterWeapon;

public:
	/*UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UArmedGameplayAbility>> DefaultAbilities;*/

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	void EquipStarterAbilities();
};