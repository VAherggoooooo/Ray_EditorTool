// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureEdtFunctions.h"

#include "AssetToolsModule.h"
#include "Editor.h"
#include "FileHandleFunctions.h"
#include "IAssetTools.h"
#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "PackageTools.h"
#include "TextureEdtLogging.h"
#include "AssetRegistry/AssetRegistryModule.h"
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


UTexture2D* UTextureEdtFunctions::CreateT2DFromRT(UTextureRenderTarget2D* RenderTarget, FString OutPath, bool bUseCustomPath, FString InName, 
	TextureCompressionSettings CompressionSettings, TextureMipGenSettings MipSettings)
{
#if WITH_EDITOR
	if (!RenderTarget)
	{
		return nullptr;
	}
	else if (!RenderTarget->GetResource())
	{
		return nullptr;
	}
	else
	{
		FString Name;
		FString PackageName;
		IAssetTools& AssetTools = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		//Use asset name only if directories are specified, otherwise full path
		if (!InName.Contains(TEXT("/")))
		{
			FString _Path = "";
			if(!bUseCustomPath)
			{
				FString AssetName = RenderTarget->GetOutermost()->GetName();
				const FString SanitizedBasePackageName = UPackageTools::SanitizePackageName(AssetName);
				_Path = FPackageName::GetLongPackagePath(SanitizedBasePackageName) + TEXT("/");
			}
			else
			{
				_Path = OutPath;
			}
			
			const FString PackagePath = _Path;
			AssetTools.CreateUniqueAssetName(PackagePath, InName, PackageName, Name);
		}
		else
		{
			InName.RemoveFromStart(TEXT("/"));
			InName.RemoveFromStart(TEXT("Content/"));
			InName.StartsWith(TEXT("Game/")) == true ? InName.InsertAt(0, TEXT("/")) : InName.InsertAt(0, TEXT("/Game/"));
			AssetTools.CreateUniqueAssetName(InName, TEXT(""), PackageName, Name);
		}

		UObject* NewObj = nullptr;

		// create a static 2d texture
		NewObj = RenderTarget->ConstructTexture2D(CreatePackage( *PackageName), Name, RenderTarget->GetMaskedFlags() | RF_Public | RF_Standalone, CTF_Default | CTF_AllowMips | CTF_SkipPostEdit, nullptr);
		UTexture2D* NewTex = Cast<UTexture2D>(NewObj);

		if (NewTex != nullptr)
		{
			// package needs saving
			NewObj->MarkPackageDirty();

			// Update Compression and Mip settings
			NewTex->CompressionSettings = CompressionSettings;
			NewTex->MipGenSettings = MipSettings;
			NewTex->SRGB = true;
			NewTex->PostEditChange();

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(NewObj);

			return NewTex;
		}
	}
#endif
	return nullptr;
}

UTexture2D* UTextureEdtFunctions::LoadImageToT2D(const FString& ImagePath)
{
	UTexture2D* LoadTexture = nullptr;
	// 判断文件路径是否有效
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Image path is not exist!"));
		return nullptr;
	}

	TArray<uint8> RawFileData;
	// 将图片文件转换成uint8数据
	if (FFileHelper::LoadFileToArray(RawFileData, *ImagePath))
	{
		// 获取图片的IImageWrapperPtr
		TSharedPtr<IImageWrapper> ImageWrapper = UFileHandleFunctions::GetImageWrapper(ImagePath);
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.GetAllocatedSize()))//SourceImageData.GetAllocatedSize()
		{
			// 载入图片数据
			TArray <uint8> UncompressedBGRA;        
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				// 创建一个空白纹理
				LoadTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

				// 改写纹理数据
				void* TextureData = LoadTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
				LoadTexture->PlatformData->Mips[0].BulkData.Unlock();

				// 更新纹理数据
				LoadTexture->UpdateResource();
			}
		}
	}
	return LoadTexture;
}

UTexture2D* UTextureEdtFunctions::LoadImageToT2D_Save(const FString& ImagePath)
{
	return nullptr;
}
