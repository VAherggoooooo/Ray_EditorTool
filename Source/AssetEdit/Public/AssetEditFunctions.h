#pragma once

#include "CoreMinimal.h"
#include "Editor/MaterialEditor/Public/MaterialEditingLibrary.h"

#include "AssetEditFunctions.generated.h"


UCLASS()
class ASSETEDIT_API UAssetEditFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void SetMIScalarParameter(const TArray<UObject*>& Assets, FName ParameterName, float Value, EMaterialParameterAssociation Association = EMaterialParameterAssociation::GlobalParameter);
	UFUNCTION(BlueprintCallable)
	static void SetMITextureParameter(const TArray<UObject*>& Assets, FName ParameterName, UTexture* Value, EMaterialParameterAssociation Association = EMaterialParameterAssociation::GlobalParameter);


	UFUNCTION(BlueprintCallable)
	static void CombineTexture(const TArray<UObject*>& Assets, int SizeX = 512, int SizeY = 512, bool bDrawOnly = false);
};
