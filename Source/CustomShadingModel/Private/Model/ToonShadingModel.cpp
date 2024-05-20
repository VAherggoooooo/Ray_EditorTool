// Fill out your copyright notice in the Description page of Project Settings.

#include "Model/ToonShadingModel.h"
#include "Engine/Engine.h"
#include "MaterialShared.h"
#include "Materials/MaterialExpression.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceConstant.h"
#include "MaterialCompiler.h"
#if WITH_EDITOR
#include "MaterialGraph/MaterialGraphNode_Comment.h"
#include "MaterialGraph/MaterialGraphNode.h"
#include "MaterialGraph/MaterialGraphSchema.h"
#endif //WITH_EDITOR

#define LOCTEXT_NAMESPACE "ToonShadingModelOutput"

UToonShadingModelOutput::UToonShadingModelOutput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_Utility;
		FConstructorStatics(const FString& DisplayName, const FString& FunctionName)
			: NAME_Utility(LOCTEXT("Utility", "Utility"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics(GetDisplayName(), GetFunctionName());

	MenuCategories.Add(ConstructorStatics.NAME_Utility);

	bCollapsed = true;

	// No outputs
	Outputs.Reset();
#endif
}

#if WITH_EDITOR

int32 UToonShadingModelOutput::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	auto CompileExpressionInput = [&](FExpressionInput& ExpressionInput, float DefaultValue)
	{
		if (ExpressionInput.GetTracedInput().Expression)
		{
			return Compiler->CustomOutput(this, OutputIndex, ExpressionInput.Compile(Compiler));
		}
		return Compiler->CustomOutput(this, OutputIndex, Compiler->Constant(DefaultValue));
	};
	
	switch (EToonShadingModelOutput(OutputIndex))
	{
	case EToonShadingModelOutput::SpecularRange:
		return CompileExpressionInput(SpecularRange, 0.f);
	case EToonShadingModelOutput::Offset:
		return CompileExpressionInput(Offset, 1.f);
	}
	return CompilerError(Compiler, TEXT("Input missing"));
}

void UToonShadingModelOutput::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(FString(TEXT("ToonShading")));
}

uint32 UToonShadingModelOutput::GetInputType(int32 InputIndex)
{
	switch (EToonShadingModelOutput(InputIndex))
	{
	case EToonShadingModelOutput::SpecularRange:
		return MCT_Float1;
	case EToonShadingModelOutput::Offset:
		return MCT_Float1;
	}
	return MCT_Float1;
}

FExpressionInput* UToonShadingModelOutput::GetInput(int32 InputIndex)
{
	switch (EToonShadingModelOutput(InputIndex))
	{
	case EToonShadingModelOutput::SpecularRange:
		return &SpecularRange;
	case EToonShadingModelOutput::Offset:
		return &Offset;
	}
	return &Input;
}
#endif // WITH_EDITOR


#undef LOCTEXT_NAMESPACE