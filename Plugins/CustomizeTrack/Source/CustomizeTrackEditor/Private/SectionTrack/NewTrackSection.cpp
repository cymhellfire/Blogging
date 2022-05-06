#include "SectionTrack/NewTrackSection.h"
#include "SequencerSectionPainter.h"
#include "EditorWidgets/Public/SDropTarget.h"

FNewTrackSection::FNewTrackSection(UMovieSceneSection& InSectionObject)
	: FSequencerSection(InSectionObject)
{
}

int32 FNewTrackSection::OnPaintSection(FSequencerSectionPainter& Painter) const
{
	return Painter.PaintSectionBackground();
}

FText FNewTrackSection::GetSectionTitle() const
{
	return FText::FromString(TEXT("NewSection"));
}
