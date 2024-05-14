// Fill out your copyright notice in the Description page of Project Settings.


#include "FileHandleFunctions.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "FileHelpers.h"


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
