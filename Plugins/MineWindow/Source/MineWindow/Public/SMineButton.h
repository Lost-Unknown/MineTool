// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SButton.h"

/**
 * 
 */
class MINEWINDOW_API SMineButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(SMineButton)
		:_isMine(false)
		,_isOpened(false)
		,_Text()
		,_AdjacentMines()
	{
	}
	SLATE_ATTRIBUTE(bool, isMine)

	SLATE_ATTRIBUTE(bool, isOpened)

	SLATE_ATTRIBUTE(FText, Text)

	SLATE_ATTRIBUTE(int32, AdjacentMines)

	SLATE_EVENT(FOnClicked, OnClicked)

	SLATE_END_ARGS()

	bool bIsMine;
	bool bIsOpened;
	int32 adjacentMines;
	TSharedPtr<STextBlock> Label;
	
	static FButtonStyle DefaultStyle;
	static FButtonStyle MineStyle;
	static FButtonStyle ZeroStyle;
	static FButtonStyle NumberStyle;

	static void InitializeStyles();
	
	void Reveal();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
