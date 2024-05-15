// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextureEdtFunctions.generated.h"

/**
 * 
 */
UCLASS()
class TEXTUREEDT_API UTextureEdtFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//将多张贴图合并到RGBA
	UFUNCTION(BlueprintCallable)
	static void CombineTexture(const TArray<UObject*>& Assets, int SizeX = 512, int SizeY = 512, bool bDrawOnly = false);

	//T2D导出到本地
	UFUNCTION(BlueprintCallable)
	static bool ExportT2D(UTexture2D* Texture2D, const FString& Path/*地址用反斜杠: \  */);


	//TODO: 创建Texture2D https://isaratech.com/save-a-procedurally-generated-texture-as-a-new-asset/
	UFUNCTION(BlueprintCallable)
	static UTexture2D* CreateT2DFromRT(UTextureRenderTarget2D* RenderTarget, FString OutPath = "NULL", bool bUseCustomPath = false, FString InName = "Texture", enum TextureCompressionSettings CompressionSettings = TC_Default, enum TextureMipGenSettings MipSettings = TMGS_FromTextureGroup);
	
	UFUNCTION(BlueprintCallable)
	static UTexture2D* LoadImageToT2D(const FString& ImagePath/*完整路径包含后缀 / */);


	//TODO: 加载图片并保存 https://zhuanlan.zhihu.com/p/66890786
	UFUNCTION(BlueprintCallable)
	static UTexture2D* LoadImageToT2D_Save(const FString& ImagePath/*完整路径包含后缀 / */, FString SavePath);
};
