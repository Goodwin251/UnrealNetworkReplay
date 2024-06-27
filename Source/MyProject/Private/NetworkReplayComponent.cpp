#include "NetworkReplayComponent.h"


UNetworkReplayComponent::UNetworkReplayComponent()
{
		PrimaryComponentTick.bCanEverTick = true;
}
void UNetworkReplayComponent::BeginPlay()
{
	Super::BeginPlay();
	ReplayName = "Test";
	ReplayFriendlyName = "TestName";
}

void UNetworkReplayComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UNetworkReplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UNetworkReplayComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("End Play"));
}

void UNetworkReplayComponent::StartReplayRecord_Implementation()
{
	GetWorld()->GetGameInstance()->StartRecordingReplay(ReplayName, ReplayFriendlyName);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Start replay"));
}

void UNetworkReplayComponent::StopReplayRecord_Implementation()
{
	GetWorld()->GetGameInstance()->StopRecordingReplay();
}

void UNetworkReplayComponent::StartReplayPlaying_Implementation()
{
	GetWorld()->GetGameInstance()->PlayReplay(ReplayName, nullptr);
}

	//IP = ReplayNetworkSocket->GetMyIP();

	//UE_LOG(LogNet, Warning, TEXT("Got IP : %s"), *IP);
	//UE_LOG(LogNet, Warning, TEXT("Port: %d"), Port);
	//ReplayNetworkSocket->Init(RMI, Port);



	//if (ReplayNetworkSocket->bSocketExist) 
	//{
	//	TArray<uint8> Bytes = ReplayNetworkSocket->fStringToBytes(message);
	//	ReplayNetworkSocket->SendToAddress(Bytes, SendIP, SendPort);
	//}


