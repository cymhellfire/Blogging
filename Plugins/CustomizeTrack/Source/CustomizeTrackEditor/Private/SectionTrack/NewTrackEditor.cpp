#include "SectionTrack/NewTrackEditor.h"

#include "EditorStyleSet.h"
#include "SectionTrack/MovieSceneNewTrack.h"
#include "SectionTrack/MovieSceneNewTrackSection.h"
#include "SectionTrack/NewTrackSection.h"
#include "SequencerUtilities.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#define LOCTEXT_NAMESPACE "FNewTrackEditor"

TSharedRef<ISequencerTrackEditor> FNewTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FNewTrackEditor(InSequencer));
}

FNewTrackEditor::FNewTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

void FNewTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddNewTrack", "New Track"),
		LOCTEXT("AddNewTrackTooltip", "Adds a new track that contains new sections."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "Sequencer.Tracks.Fade"),
		FUIAction(
			FExecuteAction::CreateRaw(this, &FNewTrackEditor::HandleAddNewTrackMenuEntryExecute)
			)
		);
}

TSharedPtr<SWidget> FNewTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track,
	const FBuildEditWidgetParams& Params)
{
	return FSequencerUtilities::MakeAddButton(
		LOCTEXT("AddNewSection", "Section"),
		FOnGetContent::CreateSP(this, &FNewTrackEditor::BuildAddNewSectionMenu, Track),
		Params.NodeIsHovered, GetSequencer());
}

TSharedRef<SWidget> FNewTrackEditor::BuildAddNewSectionMenu(UMovieSceneTrack* MovieSceneTrack)
{
	FMenuBuilder MenuBuilder(true, nullptr);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddNewSection", "New Section"),
		LOCTEXT("AddNewSectionTooltip", "Add a new section."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateSP(this, &FNewTrackEditor::OnAddNewSection, MovieSceneTrack)));

	return MenuBuilder.MakeWidget();
}

void FNewTrackEditor::OnAddNewSection(UMovieSceneTrack* MovieSceneTrack)
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();
	if (FocusedMovieScene == nullptr)
	{
		return;
	}

	UMovieSceneNewTrackSection* NewSection = AddNewSection(FocusedMovieScene, MovieSceneTrack);

	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

UMovieSceneNewTrackSection* FNewTrackEditor::AddNewSection(UMovieScene* MovieScene, UMovieSceneTrack* InTrack)
{
	const FScopedTransaction Transaction(LOCTEXT("AddNewSection_Transaction", "Add new section to track."));

	InTrack->Modify();

	UMovieSceneNewTrackSection* NewSection = CastChecked<UMovieSceneNewTrackSection>(InTrack->CreateNewSection());

	TRange<FFrameNumber> SectionRange = MovieScene->GetPlaybackRange();
	NewSection->SetRange(SectionRange);

	int32 RowIndex = -1;
	for (const UMovieSceneSection* Section : InTrack->GetAllSections())
	{
		RowIndex = FMath::Max(RowIndex, Section->GetRowIndex());
	}
	NewSection->SetRowIndex(RowIndex + 1);

	InTrack->AddSection(*NewSection);

	return NewSection;
}

bool FNewTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> TrackClass) const
{
	return TrackClass == UMovieSceneNewTrack::StaticClass();
}

bool FNewTrackEditor::SupportsSequence(UMovieSceneSequence* InSequence) const
{
	// ETrackSupport TrackSupported = InSequence ? InSequence->IsTrackSupported(UMovieSceneNewTrack::StaticClass()) : ETrackSupport::NotSupported;
	// return TrackSupported == ETrackSupport::Supported;
	return true;
}

void FNewTrackEditor::HandleAddNewTrackMenuEntryExecute()
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if (FocusedMovieScene == nullptr || FocusedMovieScene->IsReadOnly())
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("AddNewTrack_Transaction", "Add New Track"));
	FocusedMovieScene->Modify();

	auto NewTrack = FocusedMovieScene->AddMasterTrack<UMovieSceneNewTrack>();
	check(NewTrack);

	auto CurSequencer = GetSequencer();
	CurSequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

TSharedRef<ISequencerSection> FNewTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject,
	UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	UMovieSceneNewTrackSection* NewTrackSection = Cast<UMovieSceneNewTrackSection>(&SectionObject);
	check(SupportsType(SectionObject.GetOuter()->GetClass()) && NewTrackSection != nullptr);

	return MakeShareable( new FNewTrackSection(*NewTrackSection));
}

#undef LOCTEXT_NAMESPACE
