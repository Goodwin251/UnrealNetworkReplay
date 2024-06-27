// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkReplayGameInstance.h"
//void UNetworkReplayGameInstance::InitSocket(bool IsRMI, int32 SetPort)
//{
// ;
//	Socket = NewObject<UNetworkReplaySocket>(this);
//	ReplayName = "ReplayName";
//	ReplayFriendlyName = "ReplayFriendlyName";
//	
//	Port = SetPort;
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Socket init"));
//	Socket->Init(IsRMI, Port);
//}
//
//void UNetworkReplayGameInstance::SendToSocket(const FString& ConnectIP, const int32 ConnectPort)
//{
//	if (Socket) 
//	{
//		Socket->SendString("TestSendToInstance", ConnectIP, ConnectPort);
//	}
//}
//
//void UNetworkReplayGameInstance::StartRecordingGame()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Start record"));
//	GetWorld()->GetGameInstance()->StartRecordingReplay(ReplayName, ReplayFriendlyName);
//}
//
//void UNetworkReplayGameInstance::StopRecordingGame()
//{
//	GetWorld()->GetGameInstance()->StopRecordingReplay();
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Recording is stopped"));
//}
//
//void UNetworkReplayGameInstance::StartGameReplay()
//{
//	PlayReplay(ReplayName, nullptr);
//	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("Port: %d"), Port));
//}
//
//
//void UNetworkReplayGameInstance::BeginDestroy()
//{
//	Super::BeginDestroy();
//	if (Socket != nullptr) 
//	{
//		Socket->bShouldReceiveData = false;
//	}
//}
