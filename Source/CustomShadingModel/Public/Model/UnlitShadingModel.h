
#pragma once

#include "CoreMinimal.h"
#include "BaseShadingModel.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpressionCustomOutput.h"
#include "UnlitShadingModel.generated.h"

/**
 * 
 */


UCLASS()
class CUSTOMSHADINGMODEL_API UUnlitShadingModelOutput : public UBaseShadingModelOutput
{
	GENERATED_UCLASS_BODY()

#if WITH_EDITOR
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual uint32 GetInputType(int32 InputIndex) override { return MCT_Float3; }
#endif
	virtual int32 GetNumOutputs() const override { return 1; }
	virtual FString GetFunctionName() const override { return TEXT("GetUnlitShading"); }
	virtual FString GetDisplayName() const override { return TEXT("UnlitShading"); }
};