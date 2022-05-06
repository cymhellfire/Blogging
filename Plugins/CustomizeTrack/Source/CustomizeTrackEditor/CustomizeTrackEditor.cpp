#include "CustomizeTrackEditor.h"

#include "ISequencerModule.h"
#include "SectionTrack/NewTrackEditor.h"
#include "KeyTrack/MovieSceneNewEventChannel.h"
#include "KeyTrack/NewEventTrackEditor.h"
#include "SequencerChannelInterface.h"
#include "Modules/ModuleManager.h"

void FCustomizeTrackEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("[CustomizeTrackEditor] Startup"));

	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");
	// Register new track editor
	NewTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FNewTrackEditor::CreateTrackEditor));
	NewEventTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FNewEventTrackEditor::CreateTrackEditor));

	SequencerModule.RegisterChannelInterface<FMovieSceneNewEventChannel>();
}

void FCustomizeTrackEditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("[CustomizeTrackEditor] Shutdown"));

	ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>( "Sequencer" );
	// Unregister callback handle
	SequencerModule.UnRegisterTrackEditor(NewTrackCreateEditorHandle);
	SequencerModule.UnRegisterTrackEditor(NewEventTrackCreateEditorHandle);
}

IMPLEMENT_MODULE(FCustomizeTrackEditorModule, CustomizeTrackEditor);
