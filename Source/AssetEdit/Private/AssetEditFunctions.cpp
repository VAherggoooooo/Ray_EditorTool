#include "AssetEditFunctions.h"

#include "CanvasTypes.h"
#include "Editor.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Runtime/Engine/Classes/Materials/Materialinstanceconstant.h"
#include "Subsystems/EditorAssetSubsystem.h"


void UAssetEditFunctions::SetMIScalarParameter(const TArray<UObject*> &Assets, FName ParameterName, float Value,
                                               EMaterialParameterAssociation Association)
{
	for (UObject* AssetData : Assets)
	{
		UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(AssetData);
		if(MIC)
		{
			UMaterialEditingLibrary::SetMaterialInstanceScalarParameterValue(MIC, ParameterName, Value, Association);
		}
	}
	
	UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	EditorAssetSubsystem->SaveLoadedAssets(Assets, false);
}

void UAssetEditFunctions::SetMITextureParameter(const TArray<UObject*>& Assets, FName ParameterName, UTexture* Value,
	EMaterialParameterAssociation Association)
{
	for (UObject* AssetData : Assets)
	{
		UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(AssetData);
		if(MIC)
		{
			UMaterialEditingLibrary::SetMaterialInstanceTextureParameterValue(MIC, ParameterName, Value, Association);
		}
	}
	
	UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	EditorAssetSubsystem->SaveLoadedAssets(Assets, false);
}


void UAssetEditFunctions::CombineTexture(const TArray<UObject*>& Assets, int SizeX, int SizeY, bool bDrawOnly)
{
	if(Assets.Num() <=0) return;
	if(Assets.Num() > 4) UE_LOG(LogInit, Warning, TEXT("选中过多texture! 该功能只会使用前4张"))

	UWorld* World = GEditor->GetEditorWorldContext().World();
	UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("Material'/Ray_EditorTool/Material/M_CombineRGBA_Inst.M_CombineRGBA_Inst'"));
	UTextureRenderTarget2D* OutRT = LoadObject<UTextureRenderTarget2D>(nullptr, TEXT("TextureRenderTarget2D'/Ray_EditorTool/Texture/RenderTarget/OutRT_1.OutRT_1'"));
	UKismetRenderingLibrary::ClearRenderTarget2D(World, OutRT);
	OutRT->ResizeTarget(SizeX, SizeY);
	
	if(!Material)
	{
		UE_LOG(LogInit, Error, TEXT("加载Material失败"))
		return;
	}
	if(!OutRT)
	{
		UE_LOG(LogInit, Error, TEXT("加载RT失败"))
		return;
	}
	
	if(!bDrawOnly)
	{
		TArray<FString> RGBA = {"R", "G", "B", "A"};
		UMaterialInstanceConstant* ConstM = Cast<UMaterialInstanceConstant>(Material);
		if(!ConstM) return;
	
		for(int i = 0; i < 4; i++)
		{
			int ID = FMath::Min(i, Assets.Num() - 1);
			UTexture* Tex = Cast<UTexture>(Assets[ID]);
			if(Tex)
			{
				UMaterialEditingLibrary::SetMaterialInstanceTextureParameterValue(ConstM, FName(RGBA[i]), Tex);
			}
		}
		UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
		EditorAssetSubsystem->SaveLoadedAssets(Assets, false);
	}
	
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(World, OutRT, Material);
}
