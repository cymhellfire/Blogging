#include "CustomizeTrack.h"

void FCustomizeTrackModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("[CustomizeTrack] Startup."));
}

void FCustomizeTrackModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("[CustomizeTrack] Shutdown."));
}

IMPLEMENT_MODULE(FCustomizeTrackModule, CustomizeTrack);
