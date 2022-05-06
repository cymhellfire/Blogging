#include "KeyTrack/MovieSceneNewEventTrack.h"

#include "Evaluation/MovieSceneEvalTemplate.h"
#include "KeyTrack/MovieSceneNewEventTriggerSection.h"

#define LOCTEXT_NAMESPACE "MovieSceneNewEventTrack"

bool UMovieSceneNewEventTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return Super::SupportsType(SectionClass);
}

UMovieSceneSection* UMovieSceneNewEventTrack::CreateNewSection()
{
	return NewObject<UMovieSceneNewEventTriggerSection>(this, NAME_None, RF_Transactional);
}

FMovieSceneEvalTemplatePtr UMovieSceneNewEventTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FMovieSceneEvalTemplatePtr();
}

FText UMovieSceneNewEventTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("NewEventTrack", "NewEvents");
}

#undef LOCTEXT_NAMESPACE
