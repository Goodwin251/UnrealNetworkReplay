// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"
#include "Sockets.h"
#include "Networking.h"
#include "SocketSubsystem.h"

#include "MyActorComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class MYPROJECT_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()



public:	
	// Sets default values for this component's properties
	UMyActorComponent();

	FSocket* RecieveSocket;
	FSocket* SendSocket;
protected:
	UPROPERTY()
	FString ReplayName;
	
	UPROPERTY()
	FString ReplayFriendlyName;

	TSharedPtr<FInternetAddr> RemoteAddr;
	bool IsListneingData;
	TFuture<void> ClientConnectionFinishedFuture;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerStartRecord();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerStopRecord();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerStartReplay();




protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
