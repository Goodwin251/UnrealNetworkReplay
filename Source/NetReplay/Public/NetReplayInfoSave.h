// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NetReplayCommand.h"

#include "NetReplayInfoSave.generated.h"
/**
 * 
 */
UCLASS()
class NETREPLAY_API UNetReplayInfoSave : public USaveGame
{
	GENERATED_BODY()

public: 

	UPROPERTY(VisibleAnywhere, Category = "Replay save")
	FString SaveName;
	UPROPERTY(VisibleAnywhere, Category = "Replay save")
	int64 UserIndex;
	UPROPERTY(VisibleAnywhere, Category = "Replay save")
	FString ReplayName;
	UPROPERTY(VisibleAnywhere, Category = "Replay save")
	TArray<FKeyframe> Keyframes;
	UNetReplayInfoSave();
};
