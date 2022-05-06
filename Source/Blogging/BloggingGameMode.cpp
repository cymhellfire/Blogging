// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloggingGameMode.h"
#include "BloggingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABloggingGameMode::ABloggingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
