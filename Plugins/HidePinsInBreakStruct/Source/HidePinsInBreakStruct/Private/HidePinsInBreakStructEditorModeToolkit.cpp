// Copyright Epic Games, Inc. All Rights Reserved.

#include "HidePinsInBreakStructEditorModeToolkit.h"
#include "HidePinsInBreakStructEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "HidePinsInBreakStructEditorModeToolkit"

FHidePinsInBreakStructEditorModeToolkit::FHidePinsInBreakStructEditorModeToolkit()
{
}

void FHidePinsInBreakStructEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FHidePinsInBreakStructEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FHidePinsInBreakStructEditorModeToolkit::GetToolkitFName() const
{
	return FName("HidePinsInBreakStructEditorMode");
}

FText FHidePinsInBreakStructEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "HidePinsInBreakStructEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
