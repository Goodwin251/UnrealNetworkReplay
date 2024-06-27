// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Async/Async.h"

#include "NetReplayCommand.h"

#include "NetworkReplaySocket.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNetSocketGetString, const FString&, RecvStr);


UCLASS(ClassGroup = "NetworkReplay", meta = (BlueprintSpawnableComponent), Blueprintable)
class MYPROJECT_API UNetworkReplaySocket : public UActorComponent
{
	GENERATED_BODY()
public:
	/*Default constructor, set:
	* Socket = nullptr;
	* bShouldReceiveData = false;
	* bRMI = false;*/
	UNetworkReplaySocket();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	
	//Set BufferMaxSize to 2MB
	virtual void BeginPlay() override;
	
	//Buffer for receiving and sending data
	int32 BufferMaxSize;
	
	//UDP socket
	FSocket* Socket;
	
	//Thread for recieving commands, isn't used in RMI socket instance
	TFuture<void> SocketListeningThread;
private:	
	//Only RMI NetReplaySocket can send information, and can't recieve it.
	bool bRMI;
public:
	//Variable that repeat SocketListeningThread
	bool bShouldReceiveData;

	/*Initialisation for UDP socket
	* Depending on RMI, create two different instances of FSocket
	* RMI = true => UDP RMI Socket with send functions
	* RMI = false => UDP Client Socket with listening thread
	*/
	UFUNCTION(BlueprintCallable)
	void Init(bool RMI, const int32 Port);

	//Return local IP of mashine
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetLocalIP();
	
	//Convert FString to bytes and return array of bytes (TArray<uint8>)
	TArray<uint8> fStringToBytes(const FString& str);
	//Convert bytes to FString and return ResultString
	FString fBytesToString(const TArray<uint8>& InArray);
	
	//Send data to target address
	bool SendToAddress(const TArray<uint8>& Bytes, const FString& IP, const int32 Port);
	//Send string to target address
	UFUNCTION(BlueprintCallable)
	void SendString(const FString& Str, const FString& TargetIP, const int32 TargetPort);
	//Delegate called when recieve string
	UPROPERTY(BlueprintAssignable)
	FNetSocketGetString OnRecvStr;
	//For destroy FSocket you maybe also need to set bShouldReceiveData to false in parent UObject.
	UFUNCTION()
	void CloseReplaySocket();
};

