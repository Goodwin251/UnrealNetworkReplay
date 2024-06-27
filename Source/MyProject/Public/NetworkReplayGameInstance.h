// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "NetworkReplaySocket.h"

#include "NetworkReplayGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UNetworkReplayGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
//protected:
//	UPROPERTY()
//	UNetworkReplaySocket* Socket;
//	UPROPERTY()
//	FString IP;
//	UPROPERTY()
//	FString ReplayName;
//	UPROPERTY()
//	FString ReplayFriendlyName;
//
//public:
//	UPROPERTY(BlueprintReadWrite)
//	int Port;
//
//	
//
//	UFUNCTION(BlueprintCallable)
//	void InitSocket(const bool IsRMI, const int32 SetPort);
//	UFUNCTION(BlueprintCallable)
//	void SendToSocket(const FString& ConnectIP, const int32 ConnectPort);
//
//	UFUNCTION(BlueprintCallable)
//	void StartRecordingGame();
//	UFUNCTION(BlueprintCallable)
//	void StopRecordingGame();
//	UFUNCTION(BlueprintCallable)
//	void StartGameReplay();
//	virtual void BeginDestroy() override;
	
};
