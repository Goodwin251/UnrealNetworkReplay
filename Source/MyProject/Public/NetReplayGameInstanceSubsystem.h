// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "NetworkReplaySocket.h"
#include "NetReplayCommand.h"

#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Engine/DemoNetDriver.h"
#include <Kismet/GameplayStatics.h>
#include "Runtime/NetworkReplayStreaming/LocalFileNetworkReplayStreaming/Public/LocalFileNetworkReplayStreaming.h"
#include "NetworkReplayStreaming.h" 

#include "NetReplayGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UNetReplaySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	// Socket for communication with other players, after playback start.
	UPROPERTY()
	UNetworkReplaySocket* Socket;
		
protected: 
	//Placeholder for configurable default replay name
	UPROPERTY(BlueprintReadOnly, Category = "Replays Managment")
	FString ReplayName;

	//Placeholder for configurable default friendly replay name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replays Managment")
	FString ReplayFriendlyName;
	// Local IP of mashine, initialise during InitSocket
	UPROPERTY(BlueprintReadOnly, Category = "Replay Socket")
	FString IP;
	//Port of mashine for socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Socket")
	int Port;
	
	//Last information that socket recieve 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Socket")
	//Save recieved data from another socket
	FString RecievedPayload;
	
	UPROPERTY(BlueprintReadOnly, Category = "Replays Managment")
	int64 ReplayID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replays Managment")
	bool bRMI;
	//Ptr on streamer to interact with native methods of FLocalFileNetworkReplayStreamer
	TSharedPtr<INetworkReplayStreamer> LocalStreamer;
public:
	//Save all founded replays by FindAllReplays function
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Replays Managment")
	TArray<FReplayInfo> ReplaysInfoList;

	//Array of all client addresses, excluding RMI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Replay Socket")
	TArray<FSocketAddress> ClientsAddresses;


	//Initialise NetworkReplaySocket, depends on IsRMI Subsystem get sender or receiving socket
	UFUNCTION(BlueprintCallable, Category = "Replay Socket")
	void InitSocket(const bool IsRMI, const int32 SetPort);
	//Send string for testing purpose
	UFUNCTION(BlueprintCallable, Category = "Replay Socket")
	void SendStringToSocket(const FString& ConnectIP, const int32 ConnectPort, const FString& String);
	//Send replay command to other subsystems
	UFUNCTION(BlueprintCallable, Category = "Replay Socket")
	void SendReplayCommand(FNetReplayCommand command, FSocketAddress address);
protected:
	//Asynchronic seek all replays in folder with replays
	UFUNCTION()
	void EnumerateAllStreams();
	
	//Called once all replay files founded
	void OnEnumerateStreamsComplete(const FEnumerateStreamsResult& Results);
	//Called once replay file delete
	void OnDeleteFinishedStreamComplete(const FDeleteFinishedStreamResult& Result);
	//Called when name changed
	void OnChangeNameComplete(const FRenameReplayResult& Result);

	FEnumerateStreamsCallback OnEnumerateStreamsCompleteDelegate;
	FDeleteFinishedStreamCallback OnDeleteFinishedStreamDelegate;
	FRenameReplayCallback OnRenameReplayDelegate;

public:
	//Asynchronic seek all replays in demo folder
	UFUNCTION(BlueprintCallable, Category = "Replays Managment")
	void FindAllReplays();
	//Asynchronic delete replay by file name
	UFUNCTION(BlueprintCallable, Category = "Replays Managment")
	void DeleteReplay(const FString& name);
	//Asynchronic rename UI replay name by file name
	UFUNCTION(BlueprintCallable, Category = "Replays Managment")
	void ChangeFriendlyNameOfReplay(const FString& targetReplayName, const FString& newFriendlyName);

	//Start record of game
	UFUNCTION(BlueprintCallable, Category = "Replays Managment")
	void StartRecordingGameInBP(FString CustomReplayName = "Replay");

	UFUNCTION()
	void StartRecord();


	//Stop record of game
	UFUNCTION(BlueprintCallable, Category = "Replays Managment")
	void StopRecordingGame();

	
	//Called when socket recieve something
	UFUNCTION()
	void RecieveMessagePayload(const FString& RecvStr);

	//Hande incoming command
	UFUNCTION(BlueprintCallable, Category = "Replay control")
	void HandleCommand(FNetReplayCommand command);
	//Start replay
	UFUNCTION(BlueprintCallable, Category = "Replay control")
	void StartReplay();
	//Function to pause replay and whole game. Those who call function still can move.
	UFUNCTION(BlueprintCallable, Category = "Replay control")
	void PauseReplay(const bool DoPause);
	//Will rewind replay to passed seconds value
	UFUNCTION(BlueprintCallable, Category = "Replay control")
	void RewindTo(const int32 seconds);
	//Change speed of game
	UFUNCTION(BlueprintCallable, Category = "Replay control")
	void ChangePlayRate(const float rate);
};
