#pragma once

#include "Modules/ModuleInterface.h"

class FCustomizeTrackEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	FDelegateHandle NewTrackCreateEditorHandle;
	FDelegateHandle NewEventTrackCreateEditorHandle;
};
