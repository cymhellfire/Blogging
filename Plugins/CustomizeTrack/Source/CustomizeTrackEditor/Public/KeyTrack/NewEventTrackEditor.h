#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

class CUSTOMIZETRACKEDITOR_API FNewEventTrackEditor : public FMovieSceneTrackEditor
{
public:
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> InSequencer);

	FNewEventTrackEditor(TSharedRef<ISequencer> InSequencer);

	// ISequencerTrackEditor interface
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> TrackClass) const override;
	virtual bool SupportsSequence(UMovieSceneSequence* InSequence) const override { return true; }
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;

	virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params) override;

	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;

protected:
	void HandleAddNewTrackMenuEntryExecute();

	void CreateNewSection(UMovieSceneTrack* Track, int32 RowIndex);
};
