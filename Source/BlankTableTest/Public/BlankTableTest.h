
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FBlankTableTestModule : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
	
    void PluginButtonClicked();
	
private:

    void RegisterMenus();

private:
    TSharedPtr<class FUICommandList> PluginCommands;
};
