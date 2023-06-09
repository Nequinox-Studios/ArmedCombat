#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ArmAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UArmAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UArmAttributeSet, Health);
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData Balance;
	ATTRIBUTE_ACCESSORS(UArmAttributeSet, Balance);
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData MaxBalance;
	ATTRIBUTE_ACCESSORS(UArmAttributeSet, MaxBalance);
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData DodgeForce;
	ATTRIBUTE_ACCESSORS(UArmAttributeSet, DodgeForce);
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData AttackLength;
	ATTRIBUTE_ACCESSORS(UArmAttributeSet, AttackLength);
};