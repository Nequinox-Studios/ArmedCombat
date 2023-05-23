#include "ArmedCombat\Knight.h"
#include "AC_BaseWeapon.h"
#include "GAS/ArmedAbilitySystemComponent.h"
#include "GAS/ArmedGameplayAbility.h"
#include "GAS/ArmAttributeSet.h"


UAbilitySystemComponent* AKnight::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AKnight::PossessedBy(AController* NewController)
{

}

void AKnight::EquipStarterAbilities()
{
	if (StarterWeapon)
	{
		FActorSpawnParameters SP;
		SP.Instigator = this;
		AAC_BaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AAC_BaseWeapon>(StarterWeapon, SP);
		FName SocketName(TEXT("WeaponSocket"));
		SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);


		if (HasAuthority() && GetAbilitySystemComponent())
		{
			for (TSubclassOf<UArmedGameplayAbility>& Ability : SpawnedWeapon->GetWeaponAbilities())
			{
				GetAbilitySystemComponent()->GiveAbility(
					FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
			}

		}

	}
}

