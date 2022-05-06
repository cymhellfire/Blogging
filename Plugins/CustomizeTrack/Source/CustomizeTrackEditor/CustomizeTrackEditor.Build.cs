using UnrealBuildTool;

public class CustomizeTrackEditor : ModuleRules
{
	public CustomizeTrackEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"SlateCore",
				"Slate",
				"Sequencer",
				"EditorWidgets",
				"MovieScene",
				"EditorStyle",
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CustomizeTrack",
			});
	}
}
