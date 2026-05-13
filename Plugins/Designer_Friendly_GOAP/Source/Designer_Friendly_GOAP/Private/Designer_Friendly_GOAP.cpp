// Copyright Epic Games, Inc. All Rights Reserved.

#include "Designer_Friendly_GOAP.h"

#define LOCTEXT_NAMESPACE "FDesigner_Friendly_GOAPModule"

void FDesigner_Friendly_GOAPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDesigner_Friendly_GOAPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDesigner_Friendly_GOAPModule, Designer_Friendly_GOAP)