using UnrealBuildTool;

public class CustomizeTrack : ModuleRules
{
	public CustomizeTrack(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
			}); 
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"MovieScene",
			});
	}
}
