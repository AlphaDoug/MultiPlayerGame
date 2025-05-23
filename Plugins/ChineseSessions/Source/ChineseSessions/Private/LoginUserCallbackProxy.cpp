﻿//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "LoginUserCallbackProxy.h"


//////////////////////////////////////////////////////////////////////////
// ULoginUserCallbackProxy

ULoginUserCallbackProxy::ULoginUserCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnLoginCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
{
}

ULoginUserCallbackProxy* ULoginUserCallbackProxy::LoginUser(UObject* WorldContextObject, class APlayerController* PlayerController, FString UserID, FString UserToken, FString AuthType)
{
	ULoginUserCallbackProxy* Proxy = NewObject<ULoginUserCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->UserID = UserID;
	Proxy->UserToken = UserToken;
	Proxy->AuthType = AuthType;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void ULoginUserCallbackProxy::Activate()
{

	if (!PlayerControllerWeakPtr.IsValid())
	{
		OnFailure.Broadcast();
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerControllerWeakPtr->Player);

	if (!Player)
	{
		OnFailure.Broadcast();
		return;
	}

	auto Identity = Online::GetIdentityInterface(GetWorld());

	if (Identity.IsValid())
	{
		// 如果没有指定任何内容，则回退到默认的AuthType（认证类型）
		if (AuthType.IsEmpty())
		{
			AuthType = Identity->GetAuthType();
		}
		DelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(Player->GetControllerId(), Delegate);
		FOnlineAccountCredentials AccountCreds(AuthType, UserID, UserToken);
		Identity->Login(Player->GetControllerId(), AccountCreds);
		return;
	}

	// Fail immediately
	OnFailure.Broadcast();
}

void ULoginUserCallbackProxy::OnCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorVal)
{
	if (PlayerControllerWeakPtr.IsValid())
	{
		ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerControllerWeakPtr->Player);

		FUniqueNetIdRepl UniqueID(UserId.AsShared());

		if (Player)
		{
			auto Identity = Online::GetIdentityInterface(GetWorld());

			if (Identity.IsValid())
			{
				Identity->ClearOnLoginCompleteDelegate_Handle(Player->GetControllerId(), DelegateHandle);
			}
			Player->SetCachedUniqueNetId(UniqueID);
		}

		if (APlayerState* State = PlayerControllerWeakPtr->PlayerState)
		{
			// Update UniqueId. See also ShowLoginUICallbackProxy.cpp
			State->SetUniqueId(UniqueID);
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
