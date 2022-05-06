#include "KeyTrack/MovieSceneNewEventSystem.h"

#include "IMovieScenePlayer.h"
#include "EntitySystem/MovieSceneEntitySystemLinker.h"
#include "EntitySystem/MovieSceneInstanceRegistry.h"

DECLARE_CYCLE_STAT(TEXT("NewEvent Systems"), MovieSceneEval_NewEvents, STATGROUP_MovieSceneECS);
DECLARE_CYCLE_STAT(TEXT("Trigger NewEvents"), MovieSceneEval_TriggerNewEvents, STATGROUP_MovieSceneECS);

bool UMovieSceneNewEventSystem::HasEvents() const
{
	return EventsByRoot.Num() > 0;
}

bool UMovieSceneNewEventSystem::IsRelevantImpl(UMovieSceneEntitySystemLinker* InLinker) const
{
	return HasEvents();
}

void UMovieSceneNewEventSystem::AddEvent(UE::MovieScene::FInstanceHandle RootInstance,
	const FMovieSceneNewEventTriggerData& TriggerData)
{
	check(!TriggerData.TriggerName.IsEmpty());
	EventsByRoot.FindOrAdd(RootInstance).Add(TriggerData);
}

void UMovieSceneNewEventSystem::OnRun(FSystemTaskPrerequisites& InPrerequisites, FSystemSubsequentTasks& Subsequents)
{
	if (HasEvents())
	{
		TriggerAllEvents();
	}
}

void UMovieSceneNewEventSystem::TriggerAllEvents()
{
	using namespace UE::MovieScene;

	SCOPE_CYCLE_COUNTER(MovieSceneEval_NewEvents);

	FInstanceRegistry* InstanceRegistry = Linker->GetInstanceRegistry();
	struct FTriggerBatch
	{
		TArray<FMovieSceneNewEventTriggerData> TriggerData;
		IMovieScenePlayer* Player;
	};
	TArray<FTriggerBatch> TriggerBatches;

	for (TPair<FInstanceHandle, TArray<FMovieSceneNewEventTriggerData>>& Pair : EventsByRoot)
	{
		const FSequenceInstance& RootInstance = InstanceRegistry->GetInstance(Pair.Key);

		if (RootInstance.GetContext().GetDirection() == EPlayDirection::Forwards)
		{
			Algo::SortBy(Pair.Value, &FMovieSceneNewEventTriggerData::RootTime);
		}
		else
		{
			Algo::SortBy(Pair.Value, &FMovieSceneNewEventTriggerData::RootTime, TGreater<>());
		}

		FTriggerBatch& TriggerBatch = TriggerBatches.Emplace_GetRef();
		TriggerBatch.TriggerData = Pair.Value;
		TriggerBatch.Player = RootInstance.GetPlayer();
	}

	// We need to clean our state before actually triggering the events because one of those events could
	// call back into an evaluation (for instance, by starting play on another sequence). If we don't clean
	// this before, would would re-enter and re-trigger past events, resulting in an infinite loop!
	EventsByRoot.Empty();

	for (const FTriggerBatch& TriggerBatch : TriggerBatches)
	{
		TriggerEvents(TriggerBatch.TriggerData, TriggerBatch.Player);
	}
}

void UMovieSceneNewEventSystem::TriggerEvents(TArrayView<const FMovieSceneNewEventTriggerData> Events,
	IMovieScenePlayer* Player)
{
	for (const FMovieSceneNewEventTriggerData& Event : Events)
	{
		SCOPE_CYCLE_COUNTER(MovieSceneEval_TriggerNewEvents);

		UE_LOG(LogTemp, Log, TEXT("[NewEvent] %s"), *Event.TriggerName);
	}
}
