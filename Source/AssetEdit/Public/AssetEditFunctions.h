#pragma once

#include "CoreMinimal.h"
#include "Editor/MaterialEditor/Public/MaterialEditingLibrary.h"
#include "AssetEditFunctions.generated.h"


UCLASS()
class ASSETEDIT_API UAssetEditFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//设置材质实例float参数
	UFUNCTION(BlueprintCallable)
	static void SetMIScalarParameter(const TArray<UObject*>& Assets, FName ParameterName, float Value, EMaterialParameterAssociation Association = EMaterialParameterAssociation::GlobalParameter);

	//设置材质实例texture参数
	UFUNCTION(BlueprintCallable)
	static void SetMITextureParameter(const TArray<UObject*>& Assets, FName ParameterName, UTexture* Value, EMaterialParameterAssociation Association = EMaterialParameterAssociation::GlobalParameter);

	//将多张贴图合并到RGBA
	UFUNCTION(BlueprintCallable)
	static void CombineTexture(const TArray<UObject*>& Assets, int SizeX = 512, int SizeY = 512, bool bDrawOnly = false);

	//导出资源预览图
	UFUNCTION(BlueprintCallable)
	static void ExportThumbnail(const TArray<UObject*>& Assets, int32 OutputSize);


	static UTexture2D* GetObjThumbnail(UObject *InObject, FString OutputPath,int32 OutputSize = 128);
};
