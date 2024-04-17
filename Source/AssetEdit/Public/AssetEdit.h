#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAssetEditModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;


    TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& Assets);
    void AddToMenuEntry(FMenuBuilder& MenuBuilder);
    void AddSubMenu(FMenuBuilder& MenuBuilder);
    void TestFunction();
    
private:
    FDelegateHandle ContentBrowserExtenderDelegateHandle;
};
