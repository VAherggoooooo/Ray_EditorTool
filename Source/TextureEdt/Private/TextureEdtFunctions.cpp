// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureEdtFunctions.h"

#include "Editor.h"
#include "ImageUtils.h"
#include "TextureEdtLogging.h"
#include "Editor/MaterialEditor/Public/MaterialEditingLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/Materialinstanceconstant.h"
#include "Subsystems/EditorAssetSubsystem.h"


void UTextureEdtFunctions::CombineTexture(const TArray<UObject*>& Assets, int SizeX, int SizeY, bool bDrawOnly)
{
	if(Assets.Num() <=0) return;
	if(Assets.Num() > 4) UE_LOG(LogTextureEdt, Warning, TEXT("选中过多texture! 该功能只会使用前4张"))

	UWorld* World = GEditor->GetEditorWorldContext().World();
	UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("Material'/Ray_EditorTool/Material/M_CombineRGBA_Inst.M_CombineRGBA_Inst'"));
	UTextureRenderTarget2D* OutRT = LoadObject<UTextureRenderTarget2D>(nullptr, TEXT("TextureRenderTarget2D'/Ray_EditorTool/Texture/RenderTarget/OutRT_1.OutRT_1'"));
	UKismetRenderingLibrary::ClearRenderTarget2D(World, OutRT);
	OutRT->ResizeTarget(SizeX, SizeY);  
	
	if(!Material)
	{
		UE_LOG(LogTextureEdt, Error, TEXT("加载Material失败"))
		return;
	}
	if(!OutRT)
	{
		UE_LOG(LogTextureEdt, Error, TEXT("加载RT失败"))
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

bool UTextureEdtFunctions::ExportT2D(UTexture2D* Texture2D, const FString& Path)
{
	// setup required parameters
	TextureCompressionSettings OldCompressionSettings = Texture2D->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = Texture2D->MipGenSettings;
	bool OldSRGB = Texture2D->SRGB;
	
	Texture2D->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Texture2D->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	Texture2D->SRGB = false;
	Texture2D->UpdateResource();
	
	FTexture2DMipMap& mipmap = Texture2D->PlatformData->Mips[0];
	uint8* Data = (uint8*)mipmap.BulkData.Lock(LOCK_READ_WRITE);
	//FColor* FormatedImageData = static_cast<FColor*>(mipmap.BulkData.Lock(LOCK_READ_WRITE));
	if (Data == nullptr)
	{
		mipmap.BulkData.Unlock(); // 不释放，会卡住
		Texture2D->UpdateResource();
		return false;
	}
	
	int width = Texture2D->PlatformData->SizeX;
	int height = Texture2D->PlatformData->SizeY;
	TArray<FColor> nColors;
	
	for (int32 y = 0; y < height; y++)
	{
		for (int32 x = 0; x < width; x++)
		{
			FColor bColor;
			bColor.B = Data[(y * width + x) * 4 + 0];//B 0 - 255
			bColor.G = Data[(y * width + x) * 4 + 1];//G
			bColor.R = Data[(y * width + x) * 4 + 2];//R
			bColor.A = Data[(y * width + x) * 4 + 3];//A 
			nColors.Add(bColor);
		}
	}
	mipmap.BulkData.Unlock();
	
	// return old parameters
	Texture2D->CompressionSettings = OldCompressionSettings;
	Texture2D->MipGenSettings = OldMipGenSettings;
	Texture2D->SRGB = OldSRGB;
	
	Texture2D->UpdateResource();
	
	//获取 uint8 数据，保存
	TArray<uint8> ImgData;
	FImageUtils::CompressImageArray(width, height, nColors, ImgData);
	return FFileHelper::SaveArrayToFile(ImgData, *Path);
}
