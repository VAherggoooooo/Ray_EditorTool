// Fill out your copyright notice in the Description page of Project Settings.


#include "FileHandleFunctions.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"


bool UFileHandleFunctions::OpenExplor(const FString& Title, FString& InOutLastPath, FString& OutOpenFilenames)
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

FString UFileHandleFunctions::SelectFilePath()
{
	FString Title = "Select Output Folder";
	FString OpenedFiles;
	FString DefaultLocation = FPaths::ProjectDir();
	DefaultLocation = FPaths::ConvertRelativePathToFull(DefaultLocation);
	
	bool bOpened = false;
	bOpened = OpenExplor(Title, DefaultLocation, OpenedFiles);//打开文件浏览器
	if(!bOpened) return FString("");
	
	FString ExportFileDir = FPaths::ConvertRelativePathToFull(OpenedFiles);
	return ExportFileDir;
}
