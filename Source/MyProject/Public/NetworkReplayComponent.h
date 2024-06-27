// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"

#include "Networking.h"
#include "Sockets.h"
#include "NetworkReplaySocket.h"

#include "NetworkReplayComponent.generated.h"
//Клас для керуванням модуля реплею.

UCLASS( ClassGroup="NetworkReplay", meta = (BlueprintSpawnableComponent), Blueprintable)
class MYPROJECT_API UNetworkReplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNetworkReplayComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Replay defaults", meta = (ExposeOnSpawn = "true"))
	bool RMI;

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY()
	FString ReplayName;
	UPROPERTY()
	FString ReplayFriendlyName;
public:	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void StartReplayRecord();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void StopReplayRecord();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void StartReplayPlaying();

};


