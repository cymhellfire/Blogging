#pragma once
#include "ISequencerSection.h"

class CUSTOMIZETRACKEDITOR_API FNewTrackSection : public FSequencerSection
{
public:
	FNewTrackSection(UMovieSceneSection& InSectionObject);

	virtual int32 OnPaintSection(FSequencerSectionPainter& Painter) const override;

	virtual FText GetSectionTitle() const override;
};
