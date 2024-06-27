// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorComponent.h"

// Sets default values for this component's properties
UMyActorComponent::UMyActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	ReplayName = "Test";
	ReplayFriendlyName = "TestName";
}

void UMyActorComponent::ServerStartRecord_Implementation()
{
	GetWorld()->GetGameInstance()->StartRecordingReplay(ReplayName, ReplayFriendlyName);
}

void UMyActorComponent::ServerStopRecord_Implementation()
{
	GetWorld()->GetGameInstance()->StopRecordingReplay();
	
}

void UMyActorComponent::ServerStartReplay_Implementation()
{
	AGameStateBase* gamestate = UGameplayStatics::GetGameState(GetWorld());
	if (gamestate) 
	{
		
	}
	GetWorld()->GetGameInstance()->PlayReplay(ReplayName, nullptr);
	UE_LOG(LogDemo, Warning, TEXT("%s"), IsRunningDedicatedServer())

}







// Called when the game starts
void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

