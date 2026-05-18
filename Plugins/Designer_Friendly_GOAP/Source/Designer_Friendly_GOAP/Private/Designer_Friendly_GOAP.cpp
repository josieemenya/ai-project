// Copyright Epic Games, Inc. All Rights Reserved.

#include "Designer_Friendly_GOAP.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateBrush.h"

#define LOCTEXT_NAMESPACE "FDesigner_Friendly_GOAPModule"

static TSharedPtr<FSlateStyleSet> GOAPStyleSetInstance = nullptr;

void FDesigner_Friendly_GOAPModule::StartupModule()
{
    TSharedPtr<IPlugin> GOAPPlugin = IPluginManager::Get().FindPlugin("Designer_Friendly_GOAP");
    if (!GOAPPlugin.IsValid())
    {
        return;
    }

    FString IconDir = GOAPPlugin->GetBaseDir() / TEXT("Resources");

    GOAPStyleSetInstance = MakeShareable(new FSlateStyleSet("DesignerFriendlyGOAP_CoreStyle"));
    GOAPStyleSetInstance->SetContentRoot(IconDir);
    
    GOAPStyleSetInstance->Set("ClassIcon.PlannerComponent", new FSlateImageBrush(GOAPStyleSetInstance->RootToContentDir(TEXT("Icon_PlannerComponent.png")), FVector2D(32.f, 32.f)));
    GOAPStyleSetInstance->Set("ClassIcon.UtilityReasoner",  new FSlateImageBrush(GOAPStyleSetInstance->RootToContentDir(TEXT("Icon_UtilityReasoner.png")),  FVector2D(32.f, 32.f)));
    GOAPStyleSetInstance->Set("ClassIcon.Goal",             new FSlateImageBrush(GOAPStyleSetInstance->RootToContentDir(TEXT("Icon_Goal.png")),             FVector2D(32.f, 32.f)));
    GOAPStyleSetInstance->Set("ClassIcon.Action",           new FSlateImageBrush(GOAPStyleSetInstance->RootToContentDir(TEXT("Icon_Action.png")),           FVector2D(32.f, 32.f)));
    
    FSlateStyleRegistry::RegisterSlateStyle(*GOAPStyleSetInstance);
}

void FDesigner_Friendly_GOAPModule::ShutdownModule()
{
    if (GOAPStyleSetInstance.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*GOAPStyleSetInstance);
        GOAPStyleSetInstance.Reset();
    }
}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDesigner_Friendly_GOAPModule, Designer_Friendly_GOAP)