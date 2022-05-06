#include "SectionTrack/MovieSceneNewTrack.h"
#include "SectionTrack/MovieSceneNewTrackSection.h"
#include "SectionTrack/MovieSceneNewTrackSectionTemplate.h"
#include "Evaluation/MovieSceneEvalTemplate.h"

#define LOCTEXT_NAMESPACE "MovieSceneNewTrack"

bool UMovieSceneNewTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return SectionClass == UMovieSceneNewTrackSection::StaticClass();
}

UMovieSceneSection* UMovieSceneNewTrack::CreateNewSection()
{
	return NewObject<UMovieSceneNewTrackSection>(this, NAME_None, RF_Transactional);
}

FMovieSceneEvalTemplatePtr UMovieSceneNewTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	if (const UMovieSceneNewTrackSection* NewTrackSection = Cast<const UMovieSceneNewTrackSection>(&InSection))
	{
		return FMovieSceneNewTrackSectionTemplate(*NewTrackSection);
	}

	return FMovieSceneEvalTemplatePtr();
}

#if WITH_EDITOR
FText UMovieSceneNewTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("NewTrack", "New Track");
}
#endif

#undef LOCTEXT_NAMESPACE
