// Fill out your copyright notice in the Description page of Project Settings.


#include "FileHandleFunctions.h"
#include "DesktopPlatformModule.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "IDesktopPlatform.h"
#include "FileHelpers.h"
#include "Subsystems/EditorAssetSubsystem.h"


bool UFileHandleFunctions::OpenDirectoryExplor(const FString& Title, FString& InOutLastPath, FString& OutOpenFilenames)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool opened = false; 
	if(!DesktopPlatform) return false;
	opened = DesktopPlatform->OpenDirectoryDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			Title,
			InOutLastPath,
			OutOpenFilenames
			);
	return opened;
}

bool UFileHandleFunctions::OpenFileExplor(const FString& Title, const FString& FileTypes, FString& InOutLastPath,EFileDialogFlags::Type DialogMode, TArray<FString>& OutOpenFilenames)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool opened = false;
	if(!DesktopPlatform) return false;
	opened = DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			Title,
			InOutLastPath,
			TEXT(""),
			FileTypes,
			DialogMode,
			OutOpenFilenames);
	return opened;
}

FString UFileHandleFunctions::SelectDirectoryPath()
{
	FString Title = "Select Folder";
	FString OpenedFiles;
	FString DefaultLocation = FPaths::ProjectDir();
	DefaultLocation = FPaths::ConvertRelativePathToFull(DefaultLocation);
	
	bool bOpened = false;
	bOpened = OpenDirectoryExplor(Title, DefaultLocation, OpenedFiles);//打开文件浏览器
	if(!bOpened) return FString("");
	
	FString ExportFileDir = FPaths::ConvertRelativePathToFull(OpenedFiles);
	return ExportFileDir;
}

FString UFileHandleFunctions::SelectFilePath()
{
	FString title = "Select File";
	FString DefaultLocation(FEditorFileUtils::GetFilename(GWorld));
	TArray<FString> OpenedFiles;
	//FString FileTypes = TEXT("ags file (*.ags)|*.ags");
	FString FileTypes = TEXT("picture (*.png)|*.png");
	FString ExportFileDir;
	bool bOpened = false;
	//打开文件浏览器
	bOpened = OpenFileExplor(title,FileTypes, DefaultLocation, EFileDialogFlags::None, OpenedFiles);

	if(!bOpened)
	{
		UE_LOG(LogTemp, Error, TEXT("打开文件错误, 请检查是否存在文件或文件是否有误"));
		return FString("");
	}

	FString FilePath = FPaths::ConvertRelativePathToFull(OpenedFiles[0]);
	FPaths::ConvertRelativePathToFull(FilePath);
	
	return FilePath;
}

FString UFileHandleFunctions::GetPackagePath(UObject* InObject)
{
	FString AssetName = InObject->GetOutermost()->GetName();
	const FString SanitizedBasePackageName = UPackageTools::SanitizePackageName(AssetName);
	FString _Path = FPackageName::GetLongPackagePath(SanitizedBasePackageName) + TEXT("/");
	return _Path;
}

TSharedPtr<IImageWrapper> UFileHandleFunctions::GetImageWrapper(const FString& ImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	if (ImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (ImagePath.EndsWith(".jpg") || ImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (ImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	return nullptr;
}

FString UFileHandleFunctions::GetNameFromLocalFullPath(FString FullPath)
{
	TArray<FString> stringArray1, stringArray2;  
	FullPath.ParseIntoArray(stringArray1, TEXT("/"), false);
	stringArray1[stringArray1.Num()-1].ParseIntoArray(stringArray2, TEXT("."), false);
	if(stringArray2.Num() <= 0) return FString("");
	return stringArray2[0];
}

void UFileHandleFunctions::RunEditorUtilityWidget(FString WidgetPath)
{
	//https://docs.unrealengine.com/4.27/zh-CN/ProgrammingAndScripting/ 好东西
	UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	UObject* asset = EditorAssetSubsystem->LoadAsset(WidgetPath);
	if(asset == nullptr) return;
	UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(asset);
	if(EditorWidget == nullptr) return;
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidget);
}
