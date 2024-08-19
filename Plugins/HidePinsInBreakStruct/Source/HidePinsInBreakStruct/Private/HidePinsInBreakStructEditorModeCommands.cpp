// Copyright Epic Games, Inc. All Rights Reserved.

#include "HidePinsInBreakStructEditorModeCommands.h"
#include "HidePinsInBreakStructEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "HidePinsInBreakStructEditorModeCommands"

FHidePinsInBreakStructEditorModeCommands::FHidePinsInBreakStructEditorModeCommands()
	: TCommands<FHidePinsInBreakStructEditorModeCommands>("HidePinsInBreakStructEditorMode",
		NSLOCTEXT("HidePinsInBreakStructEditorMode", "HidePinsInBreakStructEditorModeCommands", "HidePinsInBreakStruct Editor Mode"),
		NAME_None,
		FEditorStyle::GetStyleSetName())
{
}

void FHidePinsInBreakStructEditorModeCommands::RegisterCommands()
{
	UI_COMMAND(HidePinsAction, "Hide Pins", "Checks each file & node and hides unconnected pins in Break Struct nodes",
		EUserInterfaceActionType::Button, FInputChord());
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FHidePinsInBreakStructEditorModeCommands::GetCommands()
{
	return FHidePinsInBreakStructEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
