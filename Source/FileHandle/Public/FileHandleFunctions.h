// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FileHandleFunctions.generated.h"

/**
 * 
 */
UCLASS()
class FILEHANDLE_API UFileHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool OpenExplor(const FString& Title, FString& InOutLastPath, FString& OutOpenFilenames);

	UFUNCTION(CallInEditor, BlueprintCallable)
	static FString SelectFilePath();
};


