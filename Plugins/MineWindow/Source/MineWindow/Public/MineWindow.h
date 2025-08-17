// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SMineButton.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMineWindowModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedPtr<SEditableTextBox> ColumnTextBox;
	TSharedPtr<SEditableTextBox> RowTextBox;
	TSharedPtr<SEditableTextBox> MineCountBox;
	TSharedPtr<SGridPanel> GridPanel;
	static TArray<FVector2D> adjacentLocs;
	TSharedPtr<STextBlock> StatusBox;
	TArray<TArray<TSharedPtr<SMineButton>>> MineButtons;

	void GameOver();

	FReply OnGridGenerated();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
