//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "AutoLoginUserCallbackProxy.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////

UAutoLoginUserCallbackProxy::UAutoLoginUserCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnLoginCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
{
}

UAutoLoginUserCallbackProxy* UAutoLoginUserCallbackProxy::AutoLoginUser(UObject* WorldContextObject, int32 LocalUserNum)
{
	UAutoLoginUserCallbackProxy* Proxy = NewObject<UAutoLoginUserCallbackProxy>();
	Proxy->LocalUserNumber = LocalUserNum;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UAutoLoginUserCallbackProxy::Activate()
{
	auto Identity = Online::GetIdentityInterface(GetWorld());

	if (Identity.IsValid())
	{
		DelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(LocalUserNumber, Delegate);
		Identity->AutoLogin(LocalUserNumber);
		return;
	}

	// Fail immediately
	OnFailure.Broadcast();
}

void UAutoLoginUserCallbackProxy::OnCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorVal)
{
	auto Identity = Online::GetIdentityInterface(GetWorld());

	if (Identity.IsValid())
	{
		Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, DelegateHandle);
	}

	if(APlayerController* PController = UGameplayStatics::GetPlayerController(WorldContextObject->GetWorld(), LocalUserNum))
	{
		ULocalPlayer* Player = Cast<ULocalPlayer>(PController->Player);

		FUniqueNetIdRepl uniqueId(UserId.AsShared());

		if (Player)
		{
			Player->SetCachedUniqueNetId(uniqueId);
		}

		if (APlayerState* State = PController->PlayerState)
		{
			// Update UniqueId. See also ShowLoginUICallbackProxy.cpp
			State->SetUniqueId(uniqueId);
		}
	}


	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
