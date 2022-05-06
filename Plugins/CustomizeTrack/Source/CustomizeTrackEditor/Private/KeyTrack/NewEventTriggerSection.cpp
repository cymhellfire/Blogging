#include "KeyTrack/NewEventTriggerSection.h"

#include "ISequencer.h"
#include "MovieSceneSequence.h"
#include "MovieSceneSequenceEditor.h"
#include "MovieSceneTrack.h"
#include "SequencerSectionPainter.h"
#include "Fonts/FontMeasure.h"
#include "KeyTrack/MovieSceneNewEventTriggerSection.h"
#include "MovieSceneTools/Public/CommonMovieSceneTools.h"

bool FNewEventTriggerSection::IsSectionSelected() const
{
	TSharedPtr<ISequencer> SequencerPtr = Sequencer.Pin();

	TArray<UMovieSceneTrack*> SelectedTracks;
	SequencerPtr->GetSelectedTracks(SelectedTracks);

	UMovieSceneSection* Section = WeakSection.Get();
	UMovieSceneTrack* Track = Section ? CastChecked<UMovieSceneTrack>(Section->GetOuter()) : nullptr;
	return Track && SelectedTracks.Contains(Track);
}

int32 FNewEventTriggerSection::OnPaintSection(FSequencerSectionPainter& InPainter) const
{
	int32 LayerId = InPainter.PaintSectionBackground();

	UMovieSceneNewEventTriggerSection* EventTriggerSection = Cast<UMovieSceneNewEventTriggerSection>(WeakSection.Get());
	if (!EventTriggerSection || !IsSectionSelected())
	{
		return LayerId;
	}

	UMovieSceneSequence* Sequence = EventTriggerSection->GetTypedOuter<UMovieSceneSequence>();
	FMovieSceneSequenceEditor* SequenceEditor = FMovieSceneSequenceEditor::Find(Sequence);

	const FTimeToPixel& TimeToPixelConverter = InPainter.GetTimeConverter();

	TArrayView<const FFrameNumber> Times = EventTriggerSection->EventChannel.GetData().GetTimes();
	TArrayView<FMovieSceneNewEvent> Events = EventTriggerSection->EventChannel.GetData().GetValues();
	TRange<FFrameNumber> EventSectionRange = EventTriggerSection->GetRange();
	for (int32 KeyIndex = 0; KeyIndex < Times.Num(); ++KeyIndex)
	{
		FFrameNumber EventTime = Times[KeyIndex];
		if (EventSectionRange.Contains(EventTime))
		{
			FString EventName = Events[KeyIndex].EventName;

			const float PixelPos = TimeToPixelConverter.FrameToPixel(EventTime);
			PaintEventName(InPainter, LayerId, EventName.IsEmpty() ? TEXT("None") : EventName, PixelPos, !EventName.IsEmpty());
		}
	}

	return LayerId + 3;
}

void FNewEventTriggerSection::PaintEventName(FSequencerSectionPainter& Painter, int32 LayerId,
	const FString& InEventName, float PixelPos, bool bIsValid) const
{
	static const int32   FontSize      = 10;
	static const float   BoxOffsetPx   = 10.f;
	static const TCHAR*  WarningString = TEXT("\xf071");

	const FSlateFontInfo FontAwesomeFont = FEditorStyle::Get().GetFontStyle("FontAwesome.10");
	const FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Bold", 10);
	const FLinearColor   DrawColor       = FEditorStyle::GetSlateColor("SelectionColor").GetColor(FWidgetStyle());

	TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	
	// Setup the warning size. Static since it won't ever change
	static FVector2D WarningSize    = FontMeasureService->Measure(WarningString, FontAwesomeFont);
	const  FMargin   WarningPadding = (bIsValid || InEventName.Len() == 0) ? FMargin(0.f) : FMargin(0.f, 0.f, 4.f, 0.f);
	const  FMargin   BoxPadding     = FMargin(4.0f, 2.0f);

	const FVector2D TextSize = FontMeasureService->Measure(InEventName, SmallLayoutFont);
	const FVector2D IconSize = bIsValid ? FVector2D::ZeroVector : WarningSize;
	const FVector2D PaddedIconSize = IconSize + WarningPadding.GetDesiredSize();
	const FVector2D BoxSize = FVector2D(TextSize.X + PaddedIconSize.X, FMath::Max(TextSize.Y, PaddedIconSize.Y)) + BoxPadding.GetDesiredSize();

	// Flip the text position if getting near the end of the view range
	bool bDrawLeft = (Painter.SectionGeometry.Size.X - PixelPos) < (BoxSize.X + 22.f) - BoxOffsetPx;
	float BoxPositionX = bDrawLeft ? PixelPos - BoxSize.X - BoxOffsetPx : PixelPos + BoxOffsetPx;
	if (BoxPositionX < 0.f)
	{
		BoxPositionX = 0.f;
	}

	FVector2D BoxOffset = FVector2D(BoxPositionX, Painter.SectionGeometry.Size.Y * .5f - BoxSize.Y * .5f);
	FVector2D IconOffset = FVector2D(BoxPadding.Left, BoxSize.Y * .5f - IconSize.Y * .5f);
	FVector2D TextOffset = FVector2D(IconOffset.X + PaddedIconSize.X, BoxSize.Y * .5f - TextSize.Y * .5f);

	// Draw background
	FSlateDrawElement::MakeBox(
		Painter.DrawElements,
		LayerId + 1,
		Painter.SectionGeometry.ToPaintGeometry(BoxOffset, BoxSize),
		FEditorStyle::GetBrush("WhiteBrush"),
		ESlateDrawEffect::None,
		FLinearColor::Black.CopyWithNewOpacity(0.5f)
	);

	if (!bIsValid)
	{
		// Draw a warning icon for invalid event
		FSlateDrawElement::MakeText(
			Painter.DrawElements,
			LayerId + 2,
			Painter.SectionGeometry.ToPaintGeometry(BoxOffset + IconOffset, IconSize),
			WarningString,
			FontAwesomeFont,
			Painter.bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
			FEditorStyle::GetWidgetStyle<FTextBlockStyle>("Log.Warning").ColorAndOpacity.GetSpecifiedColor()
		);
	}

	FSlateDrawElement::MakeText(
		Painter.DrawElements,
		LayerId + 2,
		Painter.SectionGeometry.ToPaintGeometry(BoxOffset + TextOffset, TextSize),
		InEventName,
		SmallLayoutFont,
		Painter.bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
		DrawColor
	);
}
