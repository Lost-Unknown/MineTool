// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineWindowStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMineWindowStyle::StyleInstance = nullptr;

void FMineWindowStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMineWindowStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMineWindowStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MineWindowStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMineWindowStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MineWindowStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MineWindow")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MineWindow.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("SweeperIcon"), Icon20x20));

	return Style;
}

void FMineWindowStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMineWindowStyle::Get()
{
	return *StyleInstance;
}
