#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBlankMenuTestModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;


private:
    void AddMenuEntry(FMenuBarBuilder& MenuBuilder);
    void AddSubMenuSection(FMenuBuilder& Builder);

public:
    void SubMenuSectionFunc(UWorld* World);
};
