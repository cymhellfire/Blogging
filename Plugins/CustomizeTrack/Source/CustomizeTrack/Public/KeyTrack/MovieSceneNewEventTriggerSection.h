#pragma once

#include "CoreMinimal.h"
#include "MovieSceneNewEventChannel.h"
#include "MovieSceneSection.h"
#include "EntitySystem/IMovieSceneEntityProvider.h"
#include "MovieSceneNewEventTriggerSection.generated.h"

UCLASS()
class CUSTOMIZETRACK_API UMovieSceneNewEventTriggerSection : public UMovieSceneSection
	, public IMovieSceneEntityProvider
{
	GENERATED_BODY()
public:

	UMovieSceneNewEventTriggerSection();

	// IMovieSceneEntityProvider
	virtual bool PopulateEvaluationFieldImpl(const TRange<FFrameNumber>& EffectiveRange, const FMovieSceneEvaluationFieldEntityMetaData& InMetaData, FMovieSceneEntityComponentFieldBuilder* OutFieldBuilder) override;
	virtual void ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker, const FEntityImportParams& Params, FImportedEntity* OutImportedEntity) override;

	UPROPERTY()
	FMovieSceneNewEventChannel EventChannel;
};

