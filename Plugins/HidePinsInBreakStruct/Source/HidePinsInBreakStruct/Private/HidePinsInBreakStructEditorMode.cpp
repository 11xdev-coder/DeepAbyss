// Copyright Epic Games, Inc. All Rights Reserved.

#include "HidePinsInBreakStructEditorMode.h"
#include "HidePinsInBreakStructEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "HidePinsInBreakStructEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/HidePinsInBreakStructSimpleTool.h"
#include "Tools/HidePinsInBreakStructInteractiveTool.h"

// step 2: register a ToolBuilder in FHidePinsInBreakStructEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "HidePinsInBreakStructEditorMode"

const FEditorModeID UHidePinsInBreakStructEditorMode::EM_HidePinsInBreakStructEditorModeId = TEXT("EM_HidePinsInBreakStructEditorMode");

FString UHidePinsInBreakStructEditorMode::SimpleToolName = TEXT("HidePinsInBreakStruct_ActorInfoTool");
FString UHidePinsInBreakStructEditorMode::InteractiveToolName = TEXT("HidePinsInBreakStruct_MeasureDistanceTool");


UHidePinsInBreakStructEditorMode::UHidePinsInBreakStructEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UHidePinsInBreakStructEditorMode::EM_HidePinsInBreakStructEditorModeId,
		LOCTEXT("ModeName", "HidePinsInBreakStruct"),
		FSlateIcon(),
		true);
}


UHidePinsInBreakStructEditorMode::~UHidePinsInBreakStructEditorMode()
{
}


void UHidePinsInBreakStructEditorMode::ActorSelectionChangeNotify()
{
}

void UHidePinsInBreakStructEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FHidePinsInBreakStructEditorModeCommands& SampleToolCommands = FHidePinsInBreakStructEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UHidePinsInBreakStructSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UHidePinsInBreakStructInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UHidePinsInBreakStructEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FHidePinsInBreakStructEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UHidePinsInBreakStructEditorMode::GetModeCommands() const
{
	return FHidePinsInBreakStructEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
