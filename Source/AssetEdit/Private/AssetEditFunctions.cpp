#include "AssetEditFunctions.h"

#include "AssetEditLogging.h"
#include "CanvasTypes.h"
#include "Editor.h"
#include "FileHandleFunctions.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ObjectTools.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
	if(Assets.Num() > 4) UE_LOG(LogAssetEdit, Warning, TEXT("选中过多texture! 该功能只会使用前4张"))

	UWorld* World = GEditor->GetEditorWorldContext().World();
	UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("Material'/Ray_EditorTool/Material/M_CombineRGBA_Inst.M_CombineRGBA_Inst'"));
	UTextureRenderTarget2D* OutRT = LoadObject<UTextureRenderTarget2D>(nullptr, TEXT("TextureRenderTarget2D'/Ray_EditorTool/Texture/RenderTarget/OutRT_1.OutRT_1'"));
	UKismetRenderingLibrary::ClearRenderTarget2D(World, OutRT);
	OutRT->ResizeTarget(SizeX, SizeY);  
	
	if(!Material)
	{
		UE_LOG(LogAssetEdit, Error, TEXT("加载Material失败"))
		return;
	}
	if(!OutRT)
	{
		UE_LOG(LogAssetEdit, Error, TEXT("加载RT失败"))
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
		UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();//GEditor获得world的方式! 其他都不好用
		EditorAssetSubsystem->SaveLoadedAssets(Assets, false);
	}
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(World, OutRT, Material);
}

void UAssetEditFunctions::ExportThumbnail(const TArray<UObject*>& Assets, int32 OutputSize)
{
	FString path = UFileHandleFunctions::SelectFilePath();
	if(path == "") return;

	//FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	//ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);//获得选中项
	
	for (const FAssetData& AssetData : Assets)
	{
		UObject* Asset = AssetData.GetAsset();
		if(Asset)
		{
			GetObjThumbnail(Asset, path, OutputSize);//导出
		}
	}
}

UTexture2D* UAssetEditFunctions::GetObjThumbnail(UObject* InObject, FString OutputPath, int32 OutputSize)
{
	int32 ImageRes = OutputSize;//TODO: 修改尺寸
	FObjectThumbnail ObjThumnail;
	ThumbnailTools::RenderThumbnail(InObject, ImageRes, ImageRes, ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush, NULL, &ObjThumnail);
	TArray<uint8> ThumnailDatat = ObjThumnail.GetUncompressedImageData();
	FString TotalFileName = FPaths::Combine(*OutputPath, *(InObject->GetName()));
	TotalFileName += ".png";
	FArchive* Ar = IFileManager::Get().CreateFileWriter(*TotalFileName);
	TArray<FColor> ImageRawColor;
	for (int i =0; i < ThumnailDatat.Num(); i += 4)
	{
		ImageRawColor.Add(FColor(ThumnailDatat[i + 2], ThumnailDatat[i + 1], ThumnailDatat[i], ThumnailDatat[i + 3]));
	}
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	ImageWrapper->SetRaw(ImageRawColor.GetData(), ImageRawColor.GetAllocatedSize(), ImageRes, ImageRes, ERGBFormat::BGRA, 8);
	const TArray64<uint8> ImageData = ImageWrapper->GetCompressed(100);
	const TArray<uint8> Convert = TArray<uint8>(ImageData);
	UTexture2D* ReTexture2d = UKismetRenderingLibrary::ImportBufferAsTexture2D(InObject->GetWorld(), Convert);//如果需要texture2D的话
	
	Ar->Serialize((void*)ImageData.GetData(), ImageData.GetAllocatedSize());//导出
	delete Ar;
	
	const FString Text = FString::Format(TEXT("Exportint {0}!"),
				{
					*(InObject->GetName())
				});
	UKismetSystemLibrary::PrintString(GEditor->GetEditorWorldContext().World(), Text);
	return ReTexture2d;
}
