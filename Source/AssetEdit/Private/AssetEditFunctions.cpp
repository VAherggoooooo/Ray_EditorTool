#include "AssetEditFunctions.h"

#include "AssetEditLogging.h"
#include "AssetToolsModule.h"
#include "CanvasTypes.h"
#include "Editor.h"
#include "FileHandleFunctions.h"
#include "IAssetTools.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
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

void UAssetEditFunctions::SetMIScalarParameter_Single(UMaterialInstanceConstant* MIC, FName ParameterName, float Value,
	EMaterialParameterAssociation Association)
{
	if(MIC)
	{
		UMaterialEditingLibrary::SetMaterialInstanceScalarParameterValue(MIC, ParameterName, Value, Association);
	}
}

void UAssetEditFunctions::SetMIVectorParameter_Single(UMaterialInstanceConstant* MIC, FName ParameterName, FVector4f Value,
	EMaterialParameterAssociation Association)
{
	if(MIC)
	{
		UMaterialEditingLibrary::SetMaterialInstanceVectorParameterValue(MIC, ParameterName, Value, Association);
	}
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

void UAssetEditFunctions::SetMITextureParameter_Single(UMaterialInstanceConstant* MIC, FName ParameterName,
	UTexture* Value, EMaterialParameterAssociation Association)
{
	if(MIC)
	{
		UMaterialEditingLibrary::SetMaterialInstanceTextureParameterValue(MIC, ParameterName, Value, Association);
	}
}


void UAssetEditFunctions::ExportThumbnail(const TArray<UObject*>& Assets, int32 OutputSize)
{
	FString path = UFileHandleFunctions::SelectDirectoryPath();
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



