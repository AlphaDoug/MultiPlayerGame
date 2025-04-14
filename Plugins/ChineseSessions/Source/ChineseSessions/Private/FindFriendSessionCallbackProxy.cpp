//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "FindFriendSessionCallbackProxy.h"


//////////////////////////////////////////////////////////////////////////
// UGetRecentPlayersCallbackProxy
DEFINE_LOG_CATEGORY(ChineseFindFriendSessionLog);

UFindFriendSessionCallbackProxy::UFindFriendSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, OnFindFriendSessionCompleteDelegate(FOnFindFriendSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnFindFriendSessionCompleted))
{
}

UFindFriendSessionCallbackProxy* UFindFriendSessionCallbackProxy::FindFriendSession(UObject* WorldContextObject, APlayerController *PlayerController, const FBPUniqueNetId &FriendUniqueNetId)
{
	UFindFriendSessionCallbackProxy* Proxy = NewObject<UFindFriendSessionCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->cUniqueNetId = FriendUniqueNetId;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UFindFriendSessionCallbackProxy::Activate()
{
	if (!cUniqueNetId.IsValid())
	{
		// Fail immediately
		UE_LOG(ChineseFindFriendSessionLog, Warning, TEXT("查找好友会话： 失败，收到了一个错误的网络唯一ID！"));
		TArray<FBlueprintSessionResult> EmptyResult;
		OnFailure.Broadcast(EmptyResult);
		return;
	}

	if (!PlayerControllerWeakPtr.IsValid())
	{
		// Fail immediately
		UE_LOG(ChineseFindFriendSessionLog, Warning, TEXT("查找好友会话： 失败，收到了一个错误的玩家控制器！"));
		TArray<FBlueprintSessionResult> EmptyResult;
		OnFailure.Broadcast(EmptyResult);
		return;
	}

	IOnlineSessionPtr Sessions = Online::GetSessionInterface(GetWorld());

	if (Sessions.IsValid())
	{	
		ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerControllerWeakPtr->Player);
		
		if (!Player)
		{
			// Fail immediately
			UE_LOG(ChineseFindFriendSessionLog, Warning, TEXT("查找好友会话： 失败，无法转换为本地玩家！"));
			TArray<FBlueprintSessionResult> EmptyResult;
			OnFailure.Broadcast(EmptyResult);
			return;
		}

		FindFriendSessionCompleteDelegateHandle = Sessions->AddOnFindFriendSessionCompleteDelegate_Handle(Player->GetControllerId(), OnFindFriendSessionCompleteDelegate);

		Sessions->FindFriendSession(Player->GetControllerId(), *cUniqueNetId.GetUniqueNetId());

		return;
	}

	// Fail immediately
	TArray<FBlueprintSessionResult> EmptyResult;
	OnFailure.Broadcast(EmptyResult);
}


void UFindFriendSessionCallbackProxy::OnFindFriendSessionCompleted(int32 LocalPlayer, bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionInfo)
{
	IOnlineSessionPtr Sessions = Online::GetSessionInterface(GetWorld());

	if (Sessions.IsValid())
		Sessions->ClearOnFindFriendSessionCompleteDelegate_Handle(LocalPlayer, FindFriendSessionCompleteDelegateHandle);

	if ( bWasSuccessful )
	{ 
		TArray<FBlueprintSessionResult> Result;

		for (auto& Sesh : SessionInfo)
		{
			if (Sesh.IsValid())
			{
				FBlueprintSessionResult BSesh;
				BSesh.OnlineResult = Sesh;
				Result.Add(BSesh);
			}
		}

		if(Result.Num() > 0)
			OnSuccess.Broadcast(Result);
		else
		{
			UE_LOG(ChineseFindFriendSessionLog, Warning, TEXT("查找好友会话： Failed, 失败，返回了一个无效的会话。"));
			OnFailure.Broadcast(Result);
		}
	}
	else
	{
		UE_LOG(ChineseFindFriendSessionLog, Warning, TEXT("查找好友会话： 失败"));
		TArray<FBlueprintSessionResult> EmptyResult;
		OnFailure.Broadcast(EmptyResult);
	}
}
