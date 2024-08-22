// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlankTableTestStyle.h"
#include "BlankTableTest.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FBlankTableTestStyle::StyleInstance = nullptr;

void FBlankTableTestStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FBlankTableTestStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FBlankTableTestStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("BlankTableTestStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FBlankTableTestStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("BlankTableTestStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Ray_EditorTool")->GetBaseDir() / TEXT("Resources"));//!!!注意这里

	Style->Set("BlankTableTest.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FBlankTableTestStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FBlankTableTestStyle::Get()
{
	return *StyleInstance;
}
