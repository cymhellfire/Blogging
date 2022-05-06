#pragma once

#include "CoreMinimal.h"
#include "MovieSceneNameableTrack.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "MovieSceneNewTrack.generated.h"

class UMovieSceneNewTrackSection;

UCLASS()
class CUSTOMIZETRACK_API UMovieSceneNewTrack: public UMovieSceneNameableTrack, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()

public:
	UMovieSceneNewTrack()
	{
#if WITH_EDITORONLY_DATA
		TrackTint = FColor::Red;
#endif
	}

	// UMovieSceneTrack interface
	virtual void AddSection(UMovieSceneSection& Section) override { Sections.Add(&Section); }
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override { return Sections; }
	virtual bool HasSection(const UMovieSceneSection& Section) const override { return Sections.Contains(&Section); }
	virtual bool IsEmpty() const override { return Sections.Num() == 0; }
	virtual void RemoveSection(UMovieSceneSection& Section) override { Sections.Remove(&Section); }
	virtual void RemoveSectionAt(int32 SectionIndex) override { Sections.RemoveAt(SectionIndex); }
	virtual bool SupportsMultipleRows() const override { return true; }

	// IMovieSceneTrackTemplateProducer interface
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;

#if WITH_EDITOR
	virtual FText GetDefaultDisplayName() const override;
#endif

protected:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;
};
