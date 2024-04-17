#include "AssetEdit.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#define LOCTEXT_NAMESPACE "FAssetEditModule"

void FAssetEditModule::StartupModule()
{
    //asset选中右键 添加委托
    if (!IsRunningCommandlet())
    {
        //获取content browser委托列表
        FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>("ContentBrowser");
        TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates =  ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
        //添加委托
        FContentBrowserMenuExtender_SelectedAssets& AddedDelegate = CBMenuExtenderDelegates.Add_GetRef(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FAssetEditModule::OnExtendContentBrowserAssetSelectionMenu));
        //ContentBrowserExtenderDelegateHandle = AddedDelegate.GetHandle();//貌似可以不要这行???
    }
}

void FAssetEditModule::ShutdownModule()
{
    //移除委托
    if (FModuleManager::Get().IsModuleLoaded("ContentBrowser"))
    {
        FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
        TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
        CBMenuExtenderDelegates.RemoveAll([this](const FContentBrowserMenuExtender_SelectedAssets& Delegate) { return Delegate.GetHandle() == ContentBrowserExtenderDelegateHandle; });
    }
}

TSharedRef<FExtender> FAssetEditModule::OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& Assets)
{
    //编写添加的拓展  https://zhuanlan.zhihu.com/p/555678981
    TSharedRef<FExtender> Extender(new FExtender());
    Extender->AddMenuExtension(
        "GetAssetActions",
        EExtensionHook::After,
        nullptr,
                FMenuExtensionDelegate::CreateRaw(this, &FAssetEditModule::AddToMenuEntry));
    return Extender;
}

void FAssetEditModule::AddToMenuEntry(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.BeginSection("AssetEditMenu", FText::FromString(TEXT("AssetEdit")));

    //添加一个action
    MenuBuilder.AddMenuEntry(
        FText::FromString(TEXT("Test Function")),
        FText::FromString(TEXT("Test Function")),
        FSlateIcon(),
        //绑定action对应回调函数
        FUIAction(FExecuteAction::CreateRaw(this, &FAssetEditModule::TestFunction)));

    
    MenuBuilder.EndSection();
}

void FAssetEditModule::TestFunction()
{
    TArray<FAssetData> SelectedAssets;
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);//获得当前内容浏览器的选中项(可以多选)
    
    for (const FAssetData& AssetData : SelectedAssets)
    {
        UObject* Asset = AssetData.GetAsset();//筛选Uobject
        if(Asset)
        {
            UE_LOG(LogTemp, Log,TEXT("Asset: %s"),*AssetData.AssetName.ToString());
        }
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FAssetEditModule, AssetEdit)