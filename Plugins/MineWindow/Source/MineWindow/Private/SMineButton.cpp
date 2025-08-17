// Fill out your copyright notice in the Description page of Project Settings.


#include "SMineButton.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FButtonStyle SMineButton::DefaultStyle;
FButtonStyle SMineButton::MineStyle;
FButtonStyle SMineButton::ZeroStyle;
FButtonStyle SMineButton::NumberStyle;

void SMineButton::InitializeStyles()
{
    // Grab engine's default button
    DefaultStyle = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");

    MineStyle = DefaultStyle;
    ZeroStyle = DefaultStyle;
    NumberStyle = DefaultStyle;

    // Fill tints
    MineStyle.Disabled.TintColor = FSlateColor(FLinearColor::Red);

    ZeroStyle.Disabled.TintColor = FSlateColor(FLinearColor::Black);

    NumberStyle.Disabled.TintColor = FSlateColor(FLinearColor::Gray);
}
void SMineButton::Construct(const FArguments& InArgs)
{
	bIsMine = InArgs._isMine.Get();
	bIsOpened = InArgs._isOpened.Get();
    adjacentMines = InArgs._AdjacentMines.Get();
	
    SButton::Construct(
        SButton::FArguments()
        .OnClicked(InArgs._OnClicked)
        [
            SAssignNew(Label,STextBlock)
                .Text(FText::FromString(TEXT("  ")))   // bind attribute directly
        ]
    );
}

void SMineButton::Reveal() {
    switch (adjacentMines) {
    case 0: Label->SetText(FText::FromString("  "));
        SetButtonStyle(&ZeroStyle);
        break;
    case 10: Label->SetText(FText::FromString("X"));
        SetButtonStyle(&MineStyle);
        Label->SetColorAndOpacity(FLinearColor::Black);
        break;
    default: Label->SetText(FText::AsNumber(adjacentMines));
        SetButtonStyle(&NumberStyle);
        Label->SetColorAndOpacity(FLinearColor::Black);
        break;
    }
    SetEnabled(false);
    bIsOpened = true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
