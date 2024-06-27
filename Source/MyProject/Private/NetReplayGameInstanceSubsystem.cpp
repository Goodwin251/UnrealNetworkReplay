// Fill out your copyrig	ht notice in the Description page of Project Settings.


#include "NetReplayGameInstanceSubsystem.h"
DEFINE_LOG_CATEGORY(LogNetReplay);


void UNetReplaySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	Socket = nullptr;
	Port = 0;
	ReplayID = 0;

	bRMI = false;
	ReplayName = "Replay";
	ReplayFriendlyName = "Replay";
		
	LocalStreamer = FNetworkReplayStreaming::Get().GetFactory().CreateReplayStreamer();
	
	ClientsAddresses = TArray<FSocketAddress>();
	ReplaysInfoList = TArray<FReplayInfo>();


	OnEnumerateStreamsCompleteDelegate = FEnumerateStreamsCallback::CreateUObject(this, &UNetReplaySubsystem::OnEnumerateStreamsComplete);
	OnDeleteFinishedStreamDelegate = FDeleteFinishedStreamCallback::CreateUObject(this, &UNetReplaySubsystem::OnDeleteFinishedStreamComplete);
	OnRenameReplayDelegate = FRenameReplayCallback::CreateUObject(this, &UNetReplaySubsystem::OnChangeNameComplete);
	

}

void UNetReplaySubsystem::Deinitialize()
{
	Super::Deinitialize();
	if(Socket != nullptr)
		Socket->CloseReplaySocket();
}

void UNetReplaySubsystem::InitSocket(const bool IsRMI, const int32 SetPort)
{
	Socket = NewObject<UNetworkReplaySocket>(this);
	bRMI = IsRMI;
	Port = SetPort;
	
	Socket->Init(IsRMI, Port);
	
	IP = Socket->GetLocalIP();

	FScriptDelegate MyScriptDelegate;
	MyScriptDelegate.BindUFunction(this, "RecieveMessagePayload");
	Socket->OnRecvStr.AddUnique(MyScriptDelegate);
}

void UNetReplaySubsystem::SendStringToSocket(const FString& ConnectIP, const int32 ConnectPort, const FString& String)
{
	if (Socket)
	{
		Socket->SendString(String, ConnectIP, ConnectPort);
	}
}

void UNetReplaySubsystem::SendReplayCommand(FNetReplayCommand command, FSocketAddress address)
{
	if (Socket)
	{
		FString JsonPayload;
		FJsonObjectConverter::UStructToJsonObjectString(command, JsonPayload, 0, 0);

		Socket->SendString(JsonPayload, address.IP, address.Port);
	}
}

void UNetReplaySubsystem::RecieveMessagePayload(const FString& RecvStr)
{
	RecievedPayload = RecvStr;

	FNetReplayCommand Command;
	FJsonObjectConverter::JsonObjectStringToUStruct(RecvStr, &Command, 0, 0);
	HandleCommand(Command);
}

void UNetReplaySubsystem::EnumerateAllStreams()
{
	LocalStreamer->EnumerateStreams(FNetworkReplayVersion(), 0, FString(), FJsonSerializableArray(), OnEnumerateStreamsCompleteDelegate); //UserIndex just 0, because doesn't matter for PC LocalStream

}


void UNetReplaySubsystem::HandleCommand(FNetReplayCommand command)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("Second: %f"), command.FloatPayload));
	switch (command.Command)
	{
	case ENetReplayCommand::PAUSE:
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("PAUSE")));
		break;
	case ENetReplayCommand::PLAY:
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("PLAY")));
		break;
	case ENetReplayCommand::REWINDTO:
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("REWIND TO")));
		break;
	case ENetReplayCommand::TOSTART:
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("START")));
		break;
	case ENetReplayCommand::RECORD:
		UE_LOG(LogNetReplay, Log, TEXT("Received command: Recording replay"));
		ReplayName = command.ReplayName;
		ReplayFriendlyName = ReplayName;
		StartRecord();
		UE_LOG(LogNetReplay, Log, TEXT("Executed command: Recording replay"));
		break;

	case ENetReplayCommand::STOPRECORD:
		UE_LOG(LogNetReplay, Log, TEXT("Received command: Stop recording replay."));
		StopRecordingGame();
		UE_LOG(LogNetReplay, Log, TEXT("Executed command: Stop recording replay."));
		break;
	}
}
void UNetReplaySubsystem::OnEnumerateStreamsComplete(const FEnumerateStreamsResult& Results)
{
	ReplaysInfoList = TArray<FReplayInfo>();
	UE_LOG(LogNetReplay, Log, TEXT("Found %d replays"), Results.FoundStreams.Num());
	for (FNetworkReplayStreamInfo res : Results.FoundStreams)
	{
		ReplaysInfoList.Add(FReplayInfo(res.Name, res.FriendlyName, res.Timestamp, res.SizeInBytes, res.LengthInMS));
	}
}

void UNetReplaySubsystem::FindAllReplays()
{
	EnumerateAllStreams();
}

void UNetReplaySubsystem::DeleteReplay(const FString& name)
{
	LocalStreamer->DeleteFinishedStream(name, OnDeleteFinishedStreamDelegate);
}

void UNetReplaySubsystem::ChangeFriendlyNameOfReplay(const FString& replayName, const FString& newFriendlyName)
{
	LocalStreamer->RenameReplayFriendlyName(replayName, newFriendlyName, OnRenameReplayDelegate);
}

void UNetReplaySubsystem::OnDeleteFinishedStreamComplete(const FDeleteFinishedStreamResult& Result)
{
	if (Result.WasSuccessful()) 
	{
		UE_LOG(LogNetReplay, Warning, TEXT("Replay succesfully deleted."));
	}
	else 
	{
		UE_LOG(LogNetReplay, Error, TEXT("Fail to delete replay."));
	}
}

void UNetReplaySubsystem::OnChangeNameComplete(const FRenameReplayResult& Result)
{
	if (Result.WasSuccessful()) 
	{
		UE_LOG(LogNetReplay, Warning, TEXT("Replay was succesfully renamed. Updating changes will take some time."));
		FindAllReplays();
	}
	else
	{
		UE_LOG(LogNetReplay, Error, TEXT("Renaming of replay failed."));
	}
}



void UNetReplaySubsystem::StartRecordingGameInBP(FString CustomReplayName)
{
	if (bRMI) 
	{
		FDateTime time = FDateTime::Now();

		TArray<FStringFormatArg> args;
		args.Add(FStringFormatArg(CustomReplayName));
		args.Add(FStringFormatArg(time.GetYear()));
		args.Add(FStringFormatArg(time.GetMonth()));
		args.Add(FStringFormatArg(time.GetDay()));
		args.Add(FStringFormatArg(time.GetHour()));
		args.Add(FStringFormatArg(time.GetMinute()));
		args.Add(FStringFormatArg(time.GetSecond()));
		ReplayID = time.ToUnixTimestamp();
		args.Add(FStringFormatArg(ReplayID));

		ReplayName = FString::Format(TEXT("{0}_{1}-{2}-{3}_{4}-{5}-{6}_{7}"), args);
		ReplayFriendlyName = ReplayName;
		float seconds = GetWorld()->GetTimeSeconds() + 3;
		FNetReplayCommand command = FNetReplayCommand(ENetReplayCommand::RECORD, seconds, ReplayName);
		
		for (FSocketAddress addr : ClientsAddresses) 
		{
			SendReplayCommand(command, addr);
		}
		StartRecord();
	}	
}

void UNetReplaySubsystem::StartRecord() //Used in BindUFunction!
{
	GetWorld()->GetGameInstance()->StartRecordingReplay(ReplayName, ReplayFriendlyName);
	UE_LOG(LogNetReplay, Log, TEXT("Recording of %s : %s has been started"), *ReplayName, *ReplayFriendlyName);
}

void UNetReplaySubsystem::StopRecordingGame()
{
	GetWorld()->GetGameInstance()->StopRecordingReplay();
	UE_LOG(LogNetReplay, Log, TEXT("Recording has been stopped")); 
}


void UNetReplaySubsystem::StartReplay()
{
	GetWorld()->GetGameInstance()->PlayReplay(ReplayName, nullptr);
	UE_LOG(LogNetReplay, Warning, TEXT("Playback of %s : %s has been started"), *ReplayName, *ReplayFriendlyName);
}

void UNetReplaySubsystem::PauseReplay(const bool DoPause)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (DoPause)
	{
		GetWorld()->GetWorldSettings()->SetPauserPlayerState(PC->PlayerState);
		GetWorld()->bIsCameraMoveableWhenPaused = true;
		if (PC->ShouldPerformFullTickWhenPaused())
			UE_LOG(LogNetReplay, Log, TEXT("Replay is paused"))
		else
			UE_LOG(LogNetReplay, Error, TEXT("PlayerController->ShouldPerformFullTickWhenPaused return false -> Spectator movement can be restricted during game pause"))
	}
	else
	{
		GetWorld()->GetWorldSettings()->SetPauserPlayerState(nullptr);
		UE_LOG(LogNetReplay, Log, TEXT("Replay is continued"));
	}
}

void UNetReplaySubsystem::RewindTo(const int32 seconds)
{
	if (GetWorld()->GetDemoNetDriver() && GetWorld()->GetDemoNetDriver()->GetDemoTotalTime() > seconds)
	{
		GetWorld()->GetDemoNetDriver()->GetDemoTotalTime();
		GetWorld()->GetDemoNetDriver()->GotoTimeInSeconds(seconds);
	}
	else
		UE_LOG(LogNetReplay, Error, TEXT("Replay is smaller than time you want to rewind or DemoNetDriver doesn't exist."));
}

void UNetReplaySubsystem::ChangePlayRate(const float rate)
{
	if (rate == 0) 
	{
		UE_LOG(LogNetReplay, Warning, TEXT("You can't set play rate at 0, you need to pause by convinient way."));
	}
	else 
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), rate);
		UE_LOG(LogNetReplay, Log, TEXT("Play rate (time dilation) set to %f"), rate);
	}
}
