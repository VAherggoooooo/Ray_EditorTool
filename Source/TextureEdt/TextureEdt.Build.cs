using UnrealBuildTool;

public class TextureEdt : ModuleRules
{
    public TextureEdt(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AssetTools",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "UnrealEd",
                "MaterialEditor", 
                "FileHandle",
                "ImageWrapper",
                "CoreUObject",
            }
        );
    }
}