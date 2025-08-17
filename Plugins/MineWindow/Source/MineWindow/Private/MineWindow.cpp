// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineWindow.h"
#include "MineWindowStyle.h"
#include "MineWindowCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "SMineButton.h"
#include "ToolMenus.h"

static const FName MineWindowTabName("MineWindow");

#define LOCTEXT_NAMESPACE "FMineWindowModule"

void FMineWindowModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMineWindowStyle::Initialize();
	FMineWindowStyle::ReloadTextures();

	FMineWindowCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMineWindowCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMineWindowModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMineWindowModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MineWindowTabName, FOnSpawnTab::CreateRaw(this, &FMineWindowModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMineWindowTabTitle", "MineWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMineWindowModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMineWindowStyle::Shutdown();

	FMineWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MineWindowTabName);
}

TSharedRef<SDockTab> FMineWindowModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
						.Text(LOCTEXT("Mine", "Mine Sweeper"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(5.f)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								[
									SNew(STextBlock)
										.Text(LOCTEXT("FColText", "Column : "))
								]
								+ SHorizontalBox::Slot()
								[
									SAssignNew(ColumnTextBox, SEditableTextBox)
										.Text(LOCTEXT("FCols", "5"))
								]
						]
					+ SHorizontalBox::Slot()
						.Padding(5.f)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								[
									SNew(STextBlock)
										.Text(LOCTEXT("FRowText", "Rows : "))
								]
								+ SHorizontalBox::Slot()
								[
									SAssignNew(RowTextBox, SEditableTextBox)
										.Text(LOCTEXT("FRows", "5"))
								]
						]

				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(5.f)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								[
									SNew(STextBlock)
										.Text(LOCTEXT("FMineText", "No. Of Mines : "))
								]
								+ SHorizontalBox::Slot()
								[
									SAssignNew(MineCountBox, SEditableTextBox)
										.Text(LOCTEXT("FMines", "5"))
								]
						]
				]
			+SVerticalBox::Slot()
				.Padding(10.f)
				.MaxHeight(30.f)
				[
					SNew(SButton)
						.Text(LOCTEXT("Text","Generate New Grid / Restart"))
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked_Raw(this,&FMineWindowModule::OnGridGenerated)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.MaxHeight(30.f)
				[
					SAssignNew(StatusBox, STextBlock)
				]
			+SVerticalBox::Slot()
				[
					SAssignNew(GridPanel,SGridPanel)
				]
		];
}

void FMineWindowModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MineWindowTabName);
}

TArray<FVector2D> FMineWindowModule::adjacentLocs = { (FVector2D(-1, -1))
		,(FVector2D(-1, 0))
		,(FVector2D(-1, 1))
		,(FVector2D(0, -1))
		,(FVector2D(0, 1))
		,(FVector2D(1, -1))
		,(FVector2D(1, 0))
		,(FVector2D(1, 1))
};

FReply FMineWindowModule::OnGridGenerated() {
	//Getting Stats out of Input Fields
	int32 Cols = FCString::Atoi(*ColumnTextBox->GetText().ToString());
	int32 Rows = FCString::Atoi(*RowTextBox->GetText().ToString());
	int32 Mines = FCString::Atoi(*MineCountBox->GetText().ToString());
	StatusBox->SetText(FText::FromString(""));
	TArray<FVector2D> MinePos;

	//Clearing Previous Grid If Any
	SMineButton::InitializeStyles();
	MineButtons.Empty();
	GridPanel->ClearChildren();
	
	//Setting the grid size in array
	MineButtons.SetNum(Cols);
	for (int x = 0; x < Cols; x++) {
		MineButtons[x].SetNum(Rows);
	}

	//Generating Mine Tile Vectors
	for (int32 Mine = 0; Mine < Mines; Mine++) {
		FVector2D MineVector = FVector2D(FMath::RandRange(0, Cols-1),FMath::RandRange(0,Rows-1));
		while(MinePos.Contains(MineVector)) {
			MineVector = FVector2D(FMath::RandRange(0, Cols-1), FMath::RandRange(0, Rows-1));
		}
		MinePos.Add(MineVector);
	}

	//Generating Grid
	for (int32 Col = 0; Col < Cols; Col++) {
		for (int32 Row = 0; Row < Rows; Row++) {
			FVector2D CurrentVector = FVector2D(Col, Row);
				if (MinePos.Contains(CurrentVector)){
					GridPanel->AddSlot(Col, Row)
						[
							SAssignNew(MineButtons[Col][Row], SMineButton)
								.isMine(true)
								.isOpened(false)
								.AdjacentMines(10)
								.OnClicked_Lambda([this, Col, Row]() {
								this->MineButtons[Col][Row]->Reveal();
								this->GameOver();
								return FReply::Handled(); })
						];
				}
				else {
					//Calculate the Number of Adjacent Mines for a Block
					int32 AdjacentMine = 0;
					for (int x = 0; x < adjacentLocs.Num(); x++) {
						int32 currentRow = Row + adjacentLocs[x].X;
						int32 currentCol = Col + adjacentLocs[x].Y;
						if (currentRow >= 0 && currentRow < Rows && currentCol >= 0 && currentCol < Cols) {
							FVector2D checkSlot = FVector2D(currentCol, currentRow);
							if (MinePos.Contains(checkSlot)) {
								AdjacentMine++;
							}
						}
					}
					GridPanel->AddSlot(Col, Row)
						[
							SAssignNew(MineButtons[Col][Row], SMineButton)
								.isMine(false)
								.isOpened(false)
								.AdjacentMines(AdjacentMine)
								.OnClicked_Lambda([this,Col,Row,Cols,Rows,AdjacentMine](){

								//OnClick Revel Button and if No Mines Near , Reaveal Adjacent Blocks
								
								this->MineButtons[Col][Row]->Reveal();
								if (this->MineButtons[Col][Row]->adjacentMines == 0) {
									this->MineButtons[Col][Row]->Label->SetText(FText::FromString(" "));
									for (int x = 0; x < adjacentLocs.Num(); x++) {
										int32 currentRow = Row + adjacentLocs[x].X;
										int32 currentCol = Col + adjacentLocs[x].Y;
										if (currentRow >= 0 && currentRow < Rows && currentCol >= 0 && currentCol < Cols) {
											if (this->MineButtons[currentCol][currentRow]->bIsOpened == false)
											{
												this->MineButtons[currentCol][currentRow]->SimulateClick();
											}
										}
									}
								}
								return FReply::Handled();
							})
						];
				}
			}
	}
	return FReply::Handled();
}

void FMineWindowModule::GameOver() {
	//For Game Over Diable All buttons and Reveal Mines
	for (TArray<TSharedPtr<SMineButton>> ButtonCol : this->MineButtons) {
		for (TSharedPtr<SMineButton> ButtonRow : ButtonCol) {
			if (!ButtonRow->bIsOpened) {
				if (ButtonRow->bIsMine)
					ButtonRow->Reveal();
				else {
					ButtonRow->SetEnabled(false);
				}
			}
		}
	}
	StatusBox->SetText(FText::FromString("Game Over Please Restart"));
}

void FMineWindowModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMineWindowCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMineWindowCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMineWindowModule, MineWindow)