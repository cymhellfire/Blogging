#include "KeyTrack/NewEventTrackEditor.h"

#include "MovieSceneSequenceEditor.h"
#include "SequencerUtilities.h"
#include "KeyTrack/MovieSceneNewEventTrack.h"
#include "KeyTrack/MovieSceneNewEventTriggerSection.h"
#include "KeyTrack/NewEventTriggerSection.h"

#define LOCTEXT_NAMESPACE "FEventTrackEditor"

TSharedRef<ISequencerTrackEditor> FNewEventTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FNewEventTrackEditor(InSequencer));
}

FNewEventTrackEditor::FNewEventTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

bool FNewEventTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> TrackClass) const
{
	return TrackClass == UMovieSceneNewEventTrack::StaticClass();
}

void FNewEventTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	UMovieSceneSequence* RootMovieSceneSequence = GetSequencer()->GetRootMovieSceneSequence();
	FMovieSceneSequenceEditor* SequenceEditor = FMovieSceneSequenceEditor::Find(RootMovieSceneSequence);

	if (SequenceEditor && SequenceEditor->SupportsEvents(RootMovieSceneSequence))
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("AddNewEventTrack", "NewEvent Track"),
			LOCTEXT("AddNewEventTooltip", "Add a new NewEvent track."),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "Sequencer.Tracks.Event"),
			FUIAction(
				FExecuteAction::CreateRaw(this, &FNewEventTrackEditor::HandleAddNewTrackMenuEntryExecute)
			)
		);
	}
}

TSharedRef<ISequencerSection> FNewEventTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject,
	UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	UMovieSceneNewEventTriggerSection* NewEventTriggerSection = Cast<UMovieSceneNewEventTriggerSection>(&SectionObject);
	check(SupportsType(SectionObject.GetOuter()->GetClass()) && NewEventTriggerSection != nullptr);

	return MakeShareable(new FNewEventTriggerSection(*NewEventTriggerSection, GetSequencer()));
}

TSharedPtr<SWidget> FNewEventTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track,
                                                                  const FBuildEditWidgetParams& Params)
{
	const int32 RowIndex = Params.TrackInsertRowIndex;
	TWeakObjectPtr<UMovieSceneTrack> WeakTrack = Track;
	auto SubMenuCallback = [this, WeakTrack, RowIndex]
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		UMovieSceneTrack* TrackPtr = WeakTrack.Get();
		if (TrackPtr)
		{
			MenuBuilder.AddMenuEntry(
				LOCTEXT("AddNewEventSection", "NewEvents"),
				LOCTEXT("AddNewEventSectionTooltip", "Add a NewEvents section."),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateSP(this, &FNewEventTrackEditor::CreateNewSection, TrackPtr, RowIndex + 1))
				);
		}
		else
		{
			MenuBuilder.AddWidget(SNew(STextBlock).Text(LOCTEXT("InvalidTrack", "Track is no longer valid")), FText(), true);
		}

		return MenuBuilder.MakeWidget();
	};

	return SNew(SHorizontalBox)
	+ SHorizontalBox::Slot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	[
		FSequencerUtilities::MakeAddButton(LOCTEXT("AddSection", "Section"), FOnGetContent::CreateLambda(SubMenuCallback), Params.NodeIsHovered, GetSequencer())
	];
}

void FNewEventTrackEditor::HandleAddNewTrackMenuEntryExecute()
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if (FocusedMovieScene == nullptr || FocusedMovieScene->IsReadOnly())
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("Sequencer", "AddNewEventTrack_Transaction"));
	FocusedMovieScene->Modify();

	UMovieSceneNewEventTrack* NewEventTrack = FocusedMovieScene->AddMasterTrack<UMovieSceneNewEventTrack>();
	check(NewEventTrack);

	CreateNewSection(NewEventTrack, 0);

	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

void FNewEventTrackEditor::CreateNewSection(UMovieSceneTrack* Track, int32 RowIndex)
{
	FScopedTransaction Transaction(LOCTEXT("CreateNewSection", "Add Section"));

	UMovieSceneNewEventTriggerSection* NewSection = NewObject<UMovieSceneNewEventTriggerSection>(Track);
	check(NewSection);
	NewSection->SetRange(TRange<FFrameNumber>::All());

	int32 OverlapPriority = 0;
	for (UMovieSceneSection* Section : Track->GetAllSections())
	{
		if (Section->GetRowIndex() >= RowIndex)
		{
			Section->SetRowIndex(Section->GetRowIndex() + 1);
		}
		OverlapPriority = FMath::Max(Section->GetOverlapPriority() + 1, OverlapPriority);
	}
	NewSection->SetRowIndex(RowIndex);
	NewSection->SetOverlapPriority(OverlapPriority);

	Track->Modify();
	Track->AddSection(*NewSection);
	Track->UpdateEasing();

	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);

}

#undef LOCTEXT_NAMESPACE
