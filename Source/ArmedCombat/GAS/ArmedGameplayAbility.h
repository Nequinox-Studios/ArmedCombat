#pragma once


#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArmedGameplayAbility.generated.h"

//Old input style if you're using enhanced input there is a better tag based system look at the Lyra example project.
//Enum of inputs to bind abilities to
UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	PrimaryAbility	UMETA(DisplayName = "PrimaryAbility")

};

UCLASS()
class UArmedGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	FGameplayTag InputTag;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};