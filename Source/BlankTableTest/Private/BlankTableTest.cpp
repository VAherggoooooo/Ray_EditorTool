#include "BlankTableTest.h"
#include "BlankTableTestStyle.h"
#include "BlankTableTestCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName BlankTableTestTabName("BlankTableTest");

#define LOCTEXT_NAMESPACE "FBlankTableTestModule"

void FBlankTableTestModule::StartupModule()
{
	FBlankTableTestStyle::Initialize();
	FBlankTableTestStyle::ReloadTextures();

	FBlankTableTestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FBlankTableTestCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FBlankTableTestModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBlankTableTestModule::RegisterMenus));
}

void FBlankTableTestModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FBlankTableTestStyle::Shutdown();

	FBlankTableTestCommands::Unregister();
}

void FBlankTableTestModule::PluginButtonClicked()
{
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FBlankTableTestModule::PluginButtonClicked()")),
							FText::FromString(TEXT("BlankTableTest.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FBlankTableTestModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FBlankTableTestCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FBlankTableTestCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlankTableTestModule, BlankTableTest)