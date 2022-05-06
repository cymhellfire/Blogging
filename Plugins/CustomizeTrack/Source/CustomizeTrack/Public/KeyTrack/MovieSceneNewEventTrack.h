#pragma once

#include "CoreMinimal.h"
#include "MovieSceneNameableTrack.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "MovieSceneNewEventTrack.generated.h"

UCLASS()
class CUSTOMIZETRACK_API UMovieSceneNewEventTrack : public UMovieSceneNameableTrack
	, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()
public:

	UMovieSceneNewEventTrack()
	{
#if WITH_EDITORONLY_DATA
		TrackTint = FColor::Cyan;
#endif
	}

	// UMovieSceneTrack interface
	virtual void AddSection(UMovieSceneSection& Section) override { Sections.Add(&Section); }
	virtual void RemoveSection(UMovieSceneSection& Section) override { Sections.Remove(&Section); }
	virtual void RemoveSectionAt(int32 SectionIndex) override { Sections.RemoveAt(SectionIndex); }
	virtual bool HasSection(const UMovieSceneSection& Section) const override { return Sections.Contains(&Section); }
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override { return Sections; }
	virtual bool IsEmpty() const override { return Sections.Num() == 0; }
	virtual bool SupportsMultipleRows() const override { return true; }

	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
	virtual UMovieSceneSection* CreateNewSection() override;

	// IMovieSceneTrackTemplateProducer interface
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;

#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override;
#endif
private:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;
};
