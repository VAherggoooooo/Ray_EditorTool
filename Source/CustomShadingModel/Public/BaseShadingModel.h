
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpressionCustomOutput.h"
#include "BaseShadingModel.generated.h"

/**
 * 
 */

//TODO: 看这个 https://github.com/Eragon-Brisingr/ToonShader

UCLASS()
class CUSTOMSHADINGMODEL_API UBaseShadingModelOutput : public UMaterialExpressionCustomOutput
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(meta = (RequiredInput = "true"))
	FExpressionInput Input;

#if WITH_EDITOR
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual uint32 GetInputType(int32 InputIndex) override { return MCT_Float3; }
	virtual FExpressionInput* GetInput(int32 InputIndex) override;
#endif
	virtual int32 GetNumOutputs() const override { return 1; }
	virtual FString GetFunctionName() const override { return TEXT("GetCustomShading"); }
	virtual FString GetDisplayName() const override { return TEXT("CustomShading"); }
};