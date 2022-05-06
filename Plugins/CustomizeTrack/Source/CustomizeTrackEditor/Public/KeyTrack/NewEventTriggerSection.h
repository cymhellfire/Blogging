#pragma once

#include "ISequencerSection.h"

class FNewEventTriggerSection: public FSequencerSection
{
public:
	FNewEventTriggerSection(UMovieSceneSection& InSectionObject, TWeakPtr<ISequencer> InSequencer)
		: FSequencerSection(InSectionObject)
		, Sequencer(InSequencer)
	{}

	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override;

protected:
	bool IsSectionSelected() const;
	void PaintEventName(FSequencerSectionPainter& Painter, int32 LayerId, const FString& InEventName, float PixelPos, bool bIsValid) const;

	TWeakPtr<ISequencer> Sequencer;
};
