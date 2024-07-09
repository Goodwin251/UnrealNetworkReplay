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
	STOPRECORD UMETA(DisplayName = "Stop recording of replay"),
	PLAY UMETA(DisplayName = "Play"),
	PAUSE UMETA(DisplayName = "Pause"),
	REWINDTO UMETA(DisplayName = "Rewind to"),
	TOSTART UMETA(DisplayName = "Return to start"),
	RATE UMETA(DisplayName = "Change game rate"),
	NONE UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FNetReplayCommand
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetReplay Command")
	ENetReplayCommand Command;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetReplay Command")
	FString StringPayload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetReplay Command")
	float FloatPayload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetReplay Command")
	bool BoolPayload;


	FNetReplayCommand() : StringPayload(""), Command(ENetReplayCommand::NONE), FloatPayload(0),BoolPayload(false) {}
	FNetReplayCommand(ENetReplayCommand Command) : Command(Command), StringPayload(""), FloatPayload(0), BoolPayload(false){};
	FNetReplayCommand(ENetReplayCommand Command, const FString& StringValue, float FloatValue, bool BoolValue ) : StringPayload(StringValue), Command(Command), FloatPayload(FloatValue), BoolPayload(BoolValue) {};
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay information")
	FString ReplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay information")
	FString FriendlyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay information")
	FDateTime Timestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay information")
	int64 SizeInBytes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay information")
	int32 LengthInMS;


	FReplayInfo() : ReplayName("Replay"), FriendlyName("Replay"), Timestamp(0), SizeInBytes(0), LengthInMS(0) {}
	FReplayInfo(FString NewName, FString NewFriendlyName, FDateTime NewTimestamp, int64 newSize, int32 NewLengthInMS) : 
		ReplayName(NewName), FriendlyName(NewFriendlyName), Timestamp(NewTimestamp), SizeInBytes(newSize), LengthInMS(NewLengthInMS) {}
};

USTRUCT(BlueprintType)
struct FKeyframe 
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keyframe")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keyframe")
	float Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keyframe")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keyframe")
	FString ReplayName;

	FKeyframe() : Name("Default name"), Time(0), Description("No description"), ReplayName("Replay") {}
	FKeyframe(FString name, int64 id, float time, FString description, FString replayname) : Name(name), Time(time), Description(description), ReplayName(replayname) {}
};