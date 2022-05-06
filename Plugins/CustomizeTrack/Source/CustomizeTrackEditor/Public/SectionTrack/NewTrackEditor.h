#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

class UMovieSceneNewTrackSection;

class CUSTOMIZETRACKEDITOR_API FNewTrackEditor : public FMovieSceneTrackEditor
{
public:
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> InSequencer);

	FNewTrackEditor(TSharedRef<ISequencer> InSequencer);

	// ISequencerTrackEditor interface
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params) override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> TrackClass) const override;
	virtual bool SupportsSequence(UMovieSceneSequence* InSequence) const override;

	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;

protected:
	UMovieSceneNewTrackSection* AddNewSection(UMovieScene* MovieScene, UMovieSceneTrack* InTrack);

	void HandleAddNewTrackMenuEntryExecute();
	void OnAddNewSection(UMovieSceneTrack* MovieSceneTrack);
	TSharedRef<SWidget> BuildAddNewSectionMenu(UMovieSceneTrack* MovieSceneTrack);
};
