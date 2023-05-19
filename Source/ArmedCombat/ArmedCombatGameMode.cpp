// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArmedCombatGameMode.h"
#include "ArmedCombatCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArmedCombatGameMode::AArmedCombatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
