#include "BlankMenuTest.h"

#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FBlankMenuTestModule"

void FBlankMenuTestModule::StartupModule()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> MenubarExtender = MakeShareable(new FExtender());
	MenubarExtender->AddMenuBarExtension(
		"Help", /*Extension Hook*/
		EExtensionHook::After, /* Hook Position */
		nullptr, /* Command List */
		FMenuBarExtensionDelegate::CreateRaw(this, &FBlankMenuTestModule::AddMenuEntry));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenubarExtender);
}

void FBlankMenuTestModule::ShutdownModule()
{
    
}

void FBlankMenuTestModule::AddMenuEntry(FMenuBarBuilder& MenuBuilder)
{
	// Create Sections
	MenuBuilder.AddPullDownMenu(
		FText::FromString(TEXT("Blank Menu")), /* In Menu Lable */
		FText::FromString(TEXT("Blank Menu Tips")), /* In Tool Tip */
		FNewMenuDelegate::CreateRaw(this, &FBlankMenuTestModule::AddSubMenuSection), /* Pull Down Menu */
		FName(TEXT("BlankMenuEntry")) /* Extension Hook */
		);
}

void FBlankMenuTestModule::AddSubMenuSection(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(
		FText::FromString(TEXT("Blank Sub")), /* In Menu Lable */
		FText::FromString(TEXT("Blank Sub Tooltips")), /* In Tool Tip */
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FBlankMenuTestModule::SubMenuSectionFunc, GWorld.GetReference()))
	);
}

void FBlankMenuTestModule::SubMenuSectionFunc(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("Log"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBlankMenuTestModule, BlankMenuTest)