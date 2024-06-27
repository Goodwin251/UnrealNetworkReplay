#include "NetworkReplaySocket.h"

UNetworkReplaySocket::UNetworkReplaySocket() : Super::UActorComponent(), bRMI(false), Socket(nullptr), BufferMaxSize(2 * 1024 * 1024), bShouldReceiveData(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNetworkReplaySocket::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UNetworkReplaySocket::BeginPlay()
{
	Super::BeginPlay();
}



void UNetworkReplaySocket::Init(bool RMI, const int32 Port)
{
	bRMI = RMI;
	FString Desc = bRMI ? "UDP RMI Socket" : "UDP Client Socket";
	Socket = FUdpSocketBuilder(Desc)
		.AsNonBlocking()
		.AsReusable()
		.BoundToAddress(FIPv4Address::Any)
		.BoundToPort(Port)
		.WithReceiveBufferSize(BufferMaxSize)
		.WithSendBufferSize(BufferMaxSize);
	UE_LOG(LogNetReplay, Log, TEXT("%s with port %d have been created"), *Desc, Port)
	bShouldReceiveData = true;
	SocketListeningThread = Async(EAsyncExecution::Thread, [&]() 
		{
			uint32 BufferSize = 0;
			TArray<uint8> ReceiveBuffer;
			FString ResultString;
			TSharedRef<FInternetAddr> Sender = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

			if (bRMI)
			{
				bShouldReceiveData = false;
			}
			else 
			{
				UE_LOG(LogNetReplay, Log, TEXT("Listening thread started at : %s, with port : %d"), *GetLocalIP(), Port);
			}		
				while (bShouldReceiveData)
				{
					if (!Socket)
						break;
					
					if (Socket->HasPendingData(BufferSize))
					{
						ReceiveBuffer.SetNumUninitialized(BufferSize);

						int32 Read = 0;
						Socket->RecvFrom(ReceiveBuffer.GetData(), ReceiveBuffer.Num(), Read, *Sender);
						if (Sender->GetPort() != Port)
						{
							FString message = fBytesToString(ReceiveBuffer);

							OnRecvStr.Broadcast(message);

							UE_LOG(LogNetReplay, Log, TEXT("Reserve data: %s"), *message);
						}

					}
					Socket->Wait(ESocketWaitConditions::WaitForReadOrWrite, FTimespan(100));

				}
		});

}


FString UNetworkReplaySocket::GetLocalIP()
{
	bool bCanBindAll;
	TSharedPtr<class FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
	FString MyIP = Addr->ToString(false);

	return MyIP;
}

TArray<uint8> UNetworkReplaySocket::fStringToBytes(const FString& str)
{
	TArray<uint8> ResultBytes;
	ResultBytes.Append((uint8*)TCHAR_TO_UTF8(*str), str.Len());
	return ResultBytes;
}

FString UNetworkReplaySocket::fBytesToString(const TArray<uint8>& InArray)
{
	FString ResultString;
	FFileHelper::BufferToString(ResultString, InArray.GetData(), InArray.Num());
	return ResultString;
}

void UNetworkReplaySocket::SendString(const FString& Str, const FString& TargetIP, const int32 TargetPort)
{
	TArray<uint8> Bytes = fStringToBytes(Str);
	SendToAddress(Bytes, TargetIP, TargetPort);
}

bool UNetworkReplaySocket::SendToAddress(const TArray<uint8>& Bytes, const FString& IP, const int32 Port)
{
	int32 BytesSent = 0;
	FIPv4Address addr;
	FIPv4Address::Parse(IP, addr);

	TSharedRef Iaddrref = ISocketSubsystem::Get()->CreateInternetAddr(addr.Value, Port);

	bool bIsConnected = Socket->SendTo(Bytes.GetData(), Bytes.Num(), BytesSent, *Iaddrref);
	if (bIsConnected) 
	{
		UE_LOG(LogNetReplay, Log, TEXT("%d bytes sent to %d : %s"), Bytes.Num(), Port, *IP);
		return true;
	}
		
	return false;
}



void UNetworkReplaySocket::CloseReplaySocket()
{
	bShouldReceiveData = false;
	if (Socket) 
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;	
	}
}