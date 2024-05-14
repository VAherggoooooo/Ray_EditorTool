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
};
