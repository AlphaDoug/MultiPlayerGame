﻿//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "GetRecentPlayersCallbackProxy.h"


//////////////////////////////////////////////////////////////////////////
// UGetRecentPlayersCallbackProxy
DEFINE_LOG_CATEGORY(ChineseGetRecentPlayersLog);

UGetRecentPlayersCallbackProxy::UGetRecentPlayersCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, QueryRecentPlayersCompleteDelegate(FOnQueryRecentPlayersCompleteDelegate::CreateUObject(this, &ThisClass::OnQueryRecentPlayersCompleted))
{
}

UGetRecentPlayersCallbackProxy* UGetRecentPlayersCallbackProxy::GetAndStoreRecentPlayersList(UObject* WorldContextObject, const FBPUniqueNetId& UniqueNetId)
{
	UGetRecentPlayersCallbackProxy* Proxy = NewObject<UGetRecentPlayersCallbackProxy>();
	Proxy->cUniqueNetId = UniqueNetId;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UGetRecentPlayersCallbackProxy::Activate()
{
	if (!cUniqueNetId.IsValid())
	{
		// Fail immediately
		UE_LOG(ChineseGetRecentPlayersLog, Warning, TEXT("获取最近玩家失败，收到了一个错误的唯一网络ID！"));
		TArray<FBPOnlineRecentPlayer> EmptyArray;
		OnFailure.Broadcast(EmptyArray);
		return;
	}

	IOnlineFriendsPtr Friends = Online::GetFriendsInterface(GetWorld());
	if (Friends.IsValid())
	{	
		DelegateHandle = Friends->AddOnQueryRecentPlayersCompleteDelegate_Handle(QueryRecentPlayersCompleteDelegate);

		// Testing with null namespace
		Friends->QueryRecentPlayers(*(cUniqueNetId.GetUniqueNetId()), "");
		return;
	}
	// Fail immediately
	TArray<FBPOnlineRecentPlayer> EmptyArray;
	OnFailure.Broadcast(EmptyArray);
}

void UGetRecentPlayersCallbackProxy::OnQueryRecentPlayersCompleted(const FUniqueNetId &UserID, const FString &Namespace, bool bWasSuccessful, const FString& ErrorString)
{
	
	IOnlineFriendsPtr Friends = Online::GetFriendsInterface(GetWorld());
	if (Friends.IsValid())
		Friends->ClearOnQueryRecentPlayersCompleteDelegate_Handle(DelegateHandle);


	if (bWasSuccessful)
	{
		// WHOOPS
		//IOnlineFriendsPtr Friends = Online::GetFriendsInterface();
		if (Friends.IsValid())
		{
			TArray<FBPOnlineRecentPlayer> PlayersListOut;
			TArray< TSharedRef<FOnlineRecentPlayer> > PlayerList;

			Friends->GetRecentPlayers(*(cUniqueNetId.GetUniqueNetId()), "", PlayerList);
				
			for (int32 i = 0; i < PlayerList.Num(); i++)
			{
				TSharedRef<FOnlineRecentPlayer> Player = PlayerList[i];
				FBPOnlineRecentPlayer BPF;
				BPF.DisplayName = Player->GetDisplayName();
				BPF.RealName = Player->GetRealName();
				BPF.UniqueNetId.SetUniqueNetId(Player->GetUserId());
				PlayersListOut.Add(BPF);
			}

			OnSuccess.Broadcast(PlayersListOut);
		}
	}
	else
	{
		TArray<FBPOnlineRecentPlayer> EmptyArray;
		OnFailure.Broadcast(EmptyArray);
	}
}
