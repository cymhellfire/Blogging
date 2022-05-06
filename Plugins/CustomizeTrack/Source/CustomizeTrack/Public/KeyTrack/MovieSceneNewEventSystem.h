#pragma once

#include "CoreMinimal.h"
#include "EntitySystem/MovieSceneEntitySystem.h"
#include "MovieSceneNewEventSystem.generated.h"

class IMovieScenePlayer;

USTRUCT()
struct FMovieSceneNewEventTriggerData
{
	GENERATED_BODY()

	UPROPERTY()
	FString TriggerName;

	FFrameTime RootTime;
};

UCLASS()
class CUSTOMIZETRACK_API UMovieSceneNewEventSystem : public UMovieSceneEntitySystem
{
	GENERATED_BODY()
public:

	void AddEvent(UE::MovieScene::FInstanceHandle RootInstance, const FMovieSceneNewEventTriggerData& TriggerData);

	// UMovieSceneEntitySystem interface
	virtual void OnRun(FSystemTaskPrerequisites& InPrerequisites, FSystemSubsequentTasks& Subsequents) override;
	virtual bool IsRelevantImpl(UMovieSceneEntitySystemLinker* InLinker) const override;

protected:

	bool HasEvents() const;
	void TriggerAllEvents();

private:

	static void TriggerEvents(TArrayView<const FMovieSceneNewEventTriggerData> Events, IMovieScenePlayer* Player);

private:
	TMap<UE::MovieScene::FInstanceHandle, TArray<FMovieSceneNewEventTriggerData>> EventsByRoot;
};
