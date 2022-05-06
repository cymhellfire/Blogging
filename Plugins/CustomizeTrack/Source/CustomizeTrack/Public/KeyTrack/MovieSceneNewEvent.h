#pragma once

#include "CoreMinimal.h"
#include "MovieSceneNewEvent.generated.h"

USTRUCT(BlueprintType)
struct FMovieSceneNewEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="NewEvent")
	FString EventName;
};
