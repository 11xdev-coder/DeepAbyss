// Copyright Epic Games, Inc. All Rights Reserved.

#include "HidePinsInBreakStructModule.h"
#include "HidePinsInBreakStructEditorModeCommands.h"
#include "K2Node_BreakStruct.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "HidePinsInBreakStructModule"

static const FName PluginName("Hide unconnected pins in Break Struct");

void FHidePinsInBreakStructModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FHidePinsInBreakStructEditorModeCommands::Register();

	// map custom commands
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FHidePinsInBreakStructEditorModeCommands::Get().HidePinsAction,
		FExecuteAction::CreateRaw(this, &FHidePinsInBreakStructModule::HidePinsClicked),
		FCanExecuteAction());

	// register tab
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FHidePinsInBreakStructModule::RegisterMenus));
}

// create custom tab
void FHidePinsInBreakStructModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu("MainFrame.MainMenu");
		MainMenu = MainMenu->AddSubMenu("MainMenu", NAME_None, "AutoHidePins",
			LOCTEXT("AutoHidePins", "Auto Hide Pins"), LOCTEXT("AutoHidePins_Tooltip", "Open Auto Hide Pins menu"));

		FToolMenuSection& MainMenuSection = MainMenu->FindOrAddSection("Tools");
		{
			MainMenuSection.AddMenuEntryWithCommandList(FHidePinsInBreakStructEditorModeCommands::Get().HidePinsAction, PluginCommands);
		}
	}
	
}

void FHidePinsInBreakStructModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FHidePinsInBreakStructEditorModeCommands::Unregister();
}

// when we click "Hide Pins" button
void FHidePinsInBreakStructModule::HidePinsClicked()
{
	ProcessAllBlueprints();
}

void FHidePinsInBreakStructModule::ProcessAllBlueprints()
{
	// get AssetRegistry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// filter to search for blueprints
	FARFilter Filter;
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;

	// get blueprint assets
	TArray<FAssetData> BlueprintAssets;
	AssetRegistry.GetAssets(Filter, BlueprintAssets);

	// loop through each BP asset
	for (const FAssetData AssetData : BlueprintAssets)
	{
		UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
		if(Blueprint)
		{
			ProcessBlueprint(Blueprint);
		}
	}
}

void FHidePinsInBreakStructModule::ProcessBlueprint(UBlueprint* Blueprint)
{
	if(!Blueprint || !Blueprint->GeneratedClass)
	{
		return;
	}

	// loop through each EventGraph
	for (UEdGraph* EventGraph : Blueprint->UbergraphPages)
	{
		if (EventGraph)
		{
			for (UEdGraphNode* Node : EventGraph->Nodes)
			{
				if(Node)
				{
					ProcessNode(Node);
				}
			}
		}
	}

	// loop through each FunctionGraph
	for (UEdGraph* FunctionGraph : Blueprint->FunctionGraphs)
	{
		if (FunctionGraph)
		{
			for (UEdGraphNode* Node : FunctionGraph->Nodes)
			{
				if(Node)
				{
					ProcessNode(Node);
				}
			}
		}
	}
}		

void FHidePinsInBreakStructModule::ProcessNode(UEdGraphNode* Node)
{
	if (Node->IsA<UK2Node_BreakStruct>())
	{
		UK2Node_BreakStruct* BreakStructNode = Cast<UK2Node_BreakStruct>(Node);

		// Clear any existing property list and build a new one
		//TArray<FOptionalPinFromProperty> Properties;
		//TMap<FName, FOldOptionalPinSettings> OldSettings;

		//FOptionalPinManager OptionalPinManager;
		//OptionalPinManager.RebuildPropertyList(Properties, BreakStructNode->StructType);

		// Determine which pins should be hidden
		for (UEdGraphPin* Pin : BreakStructNode->Pins)
		{
			if (Pin && Pin->LinkedTo.Num() == 0)
			{
				// Find the associated property in the updated property list
				//const FName PinName = Pin->GetFName();
				//if (const FOptionalPinFromProperty* OptionalPin = Properties.FindByPredicate([PinName](const FOptionalPinFromProperty& Prop) { return Prop.PropertyName == PinName; }))
				//{
					// Hide the pin if it's in the list of optional pins
					Pin->bHidden = true;
				//}
			}
		}

		//OptionalPinManager.RebuildPropertyList(Properties, BreakStructNode->StructType);
		
		// Refresh blueprint to reflect changes
		if (UBlueprint* Blueprint = BreakStructNode->GetBlueprint())
		{
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
		}

		UE_LOG(LogTemp, Log, TEXT("Processing Break Struct Node: %s"), *Node->GetName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHidePinsInBreakStructModule, HidePinsInBreakStructEditorMode)