// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDesktopPlatform.h"
#include "IImageWrapperModule.h"
#include "FileHandleFunctions.generated.h"

/**
 * 
 */
UCLASS()
class FILEHANDLE_API UFileHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool OpenDirectoryExplor(const FString& Title, FString& InOutLastPath, FString& OutOpenFilenames);
	static bool OpenFileExplor(const FString& Title, const FString& FileTypes, FString& InOutLastPath,EFileDialogFlags::Type DialogMode, TArray<FString>& OutOpenFilenames);
	

	//选择文件夹路径
	UFUNCTION(CallInEditor, BlueprintCallable)
	static FString SelectDirectoryPath();

	UFUNCTION(CallInEditor, BlueprintCallable)
	static FString SelectFilePath();

	UFUNCTION(CallInEditor, BlueprintCallable)
	static FString GetPackagePath(UObject* InObject);

	static TSharedPtr<IImageWrapper> GetImageWrapper(const FString& ImagePath);

	UFUNCTION(BlueprintCallable)
	static FString GetNameFromLocalFullPath(FString FullPath);

	UFUNCTION(BlueprintCallable)
	static void RunEditorUtilityWidget(FString WidgetPath);
};


