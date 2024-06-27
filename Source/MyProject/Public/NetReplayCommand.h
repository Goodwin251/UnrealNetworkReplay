// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkReplayStreaming.h"
#include "NetReplayCommand.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogNetReplay, Log, All);

UENUM(BlueprintType)
enum class ENetReplayCommand : uint8
{
	RECORD UMETA(DisplayName = "Start recording"),
	PAUSE UMETA(DisplayName = "Pause"),
	PLAY UMETA(DisplayName = "Play"),
	REWINDTO UMETA(DisplayName = "Rewind to"),
	TOSTART UMETA(DisplayName = "Return to start"),
	RATE UMETA(DisplayName = "Change game rate"),
	SYNC UMETA(DisplayName = "Synchronise replays"),
	STOPRECORD UMETA(DisplayName = "Stop recording of replay"),
	NONE UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FNetReplayCommand
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkReplay Struct")
	FString ReplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkReplay Struct")
	ENetReplayCommand Command;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkReplay Struct")
	float FloatPayload;


	FNetReplayCommand() : Command(ENetReplayCommand::NONE), FloatPayload(0), ReplayName("") {}
	FNetReplayCommand(ENetReplayCommand Command) : Command(Command), FloatPayload(0), ReplayName("") {};
	FNetReplayCommand(ENetReplayCommand Command, float Second) : Command(Command), FloatPayload(Second), ReplayName("") {};
	FNetReplayCommand(ENetReplayCommand Command, float Second, const FString& ReplayName) : Command(Command), FloatPayload(Second), ReplayName(ReplayName) {};
};

USTRUCT(BlueprintType)
struct FSocketAddress 
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket Address")
	FString IP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket Address")
	int32 Port;

	FSocketAddress() : IP(""), Port(0) {};
	FSocketAddress(const FString& IP) : IP(IP), Port(0) {};
	FSocketAddress(const FString& IP, const int32 Port) : IP(IP), Port(Port){};
};

USTRUCT(BlueprintType)
struct FReplayInfo 
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadOnly)
	FString ReplayName;

	UPROPERTY(BlueprintReadOnly)
	FString FriendlyName;

	UPROPERTY(BlueprintReadOnly)
	FDateTime Timestamp;

	UPROPERTY(BlueprintReadOnly)
	int64 SizeInBytes;

	UPROPERTY(BlueprintReadOnly)
	int32 LengthInMS;


	FReplayInfo() : ReplayName("Replay"), FriendlyName("Replay"), Timestamp(0), SizeInBytes(0), LengthInMS(0) {}
	FReplayInfo(FString NewName, FString NewFriendlyName, FDateTime NewTimestamp, int64 newSize, int32 NewLengthInMS) : 
		ReplayName(NewName), FriendlyName(NewFriendlyName), Timestamp(NewTimestamp), SizeInBytes(newSize), LengthInMS(NewLengthInMS) {}
};