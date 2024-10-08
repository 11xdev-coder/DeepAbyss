// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

/**
 * This class contains info about the full set of commands used in this editor mode.
 */
class FHidePinsInBreakStructEditorModeCommands : public TCommands<FHidePinsInBreakStructEditorModeCommands>
{
public:
	FHidePinsInBreakStructEditorModeCommands();

	virtual void RegisterCommands() override;
	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands();

	TSharedPtr<FUICommandInfo> SimpleTool;
	TSharedPtr<FUICommandInfo> InteractiveTool;

	TSharedPtr<FUICommandInfo> HidePinsAction;

protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
	
	
};
