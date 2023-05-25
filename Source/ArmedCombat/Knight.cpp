#include "ArmedCombat\Knight.h"
#include "AC_BaseWeapon.h"
#include "GameplayEffectTypes.h"


AKnight::AKnight()
{
	AbilitySystemComponent = CreateDefaultSubobject<UArmedAbilitySystemComponent>(TEXT("AbilitySystem"));

	Attributes = CreateDefaultSubobject<UArmAttributeSet>("Attributes");
}

UAbilitySystemComponent* AKnight::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AKnight::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();

	EquipStarterAbilities();



}

void AKnight::EquipStarterAbilities()
{
	if (StarterWeapon)
	{
		FActorSpawnParameters SP;
		SP.Instigator = this;
		SpawnedWeapon = GetWorld()->SpawnActor<AAC_BaseWeapon>(StarterWeapon, SP);
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

void AKnight::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle Spechandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect,1,EffectContext);

		if (Spechandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spechandle.Data.Get());
		}



		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetBalanceAttribute()).AddUObject(this, &AKnight::BalanceChanged);
	}
}

void AKnight::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UArmedGameplayAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void AKnight::BalanceChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute.GetNumericValue(Attributes) <= 0)
	{
		GetAbilitySystemComponent()->CancelAbilities(); //CancelAllAbilities();
		FallOver();
		GetWorld()->GetTimerManager().SetTimer(GetUpTimerHandle,this,&AKnight::GetUp,StayDownTime,false);
	}

}

