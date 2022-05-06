#include "KeyTrack/MovieSceneNewEventTriggerSection.h"

#include "KeyTrack/MovieSceneNewEventSystem.h"
#include "KeyTrack/MovieSceneNewEventTrack.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "EntitySystem/MovieSceneEntitySystemLinker.h"
#include "Evaluation/MovieSceneEvaluationField.h"

UMovieSceneNewEventTriggerSection::UMovieSceneNewEventTriggerSection()
{
	bSupportsInfiniteRange = true;
	SetRange(TRange<FFrameNumber>::All());

#if WITH_EDITOR
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(EventChannel, FMovieSceneChannelMetaData());
#endif
}

bool UMovieSceneNewEventTriggerSection::PopulateEvaluationFieldImpl(const TRange<FFrameNumber>& EffectiveRange,
                                                                    const FMovieSceneEvaluationFieldEntityMetaData& InMetaData, FMovieSceneEntityComponentFieldBuilder* OutFieldBuilder)
{
	const int32 MetaDataIndex = OutFieldBuilder->AddMetaData(InMetaData);

	TArrayView<const FFrameNumber> Times = EventChannel.GetData().GetTimes();
	for (int32 Index = 0; Index < Times.Num(); ++Index)
	{
		if (EffectiveRange.Contains(Times[Index]))
		{
			TRange<FFrameNumber> Range(Times[Index]);
			OutFieldBuilder->AddOneShotEntity(Range, this, Index, MetaDataIndex);
		}
	}

	return true;
}

void UMovieSceneNewEventTriggerSection::ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker,
	const FEntityImportParams& Params, FImportedEntity* OutImportedEntity)
{
	using namespace UE::MovieScene;

	const int32 EventIndex = static_cast<int32>(Params.EntityID);

	TArrayView<const FFrameNumber> Times = EventChannel.GetData().GetTimes();
	TArrayView<const FMovieSceneNewEvent> Events = EventChannel.GetData().GetValues();
	if (!ensureMsgf(Events.IsValidIndex(EventIndex), TEXT("Attempting to import an event entity for an invalid index (Index: %d, Num: %d)"), EventIndex, Events.Num()))
	{
		return;
	}

	if (Events[EventIndex].EventName.IsEmpty())
	{
		return;
	}

	UMovieSceneNewEventTrack* EventTrack = GetTypedOuter<UMovieSceneNewEventTrack>();
	const FSequenceInstance& ThisInstance = EntityLinker->GetInstanceRegistry()->GetInstance(Params.Sequence.InstanceHandle);
	FMovieSceneContext Context = ThisInstance.GetContext();

	if (Context.GetStatus() == EMovieScenePlayerStatus::Stopped || Context.IsSilent())
	{
		return;
	}

	// Get the logic executor system
	UMovieSceneNewEventSystem* EventSystem = EntityLinker->LinkSystem<UMovieSceneNewEventSystem>();

	FMovieSceneNewEventTriggerData TriggerData = {
		Events[EventIndex].EventName,
		Times[EventIndex] * Context.GetSequenceToRootTransform()
	};

	EventSystem->AddEvent(ThisInstance.GetRootInstanceHandle(), TriggerData);

	// Mimic the structure changing in order to ensure that the instantiation phase runs
	EntityLinker->EntityManager.MimicStructureChanged();
}
