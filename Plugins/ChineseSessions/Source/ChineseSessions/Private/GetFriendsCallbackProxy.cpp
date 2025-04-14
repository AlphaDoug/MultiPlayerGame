//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "GetFriendsCallbackProxy.h"

#include "Interfaces/OnlinePresenceInterface.h"


//////////////////////////////////////////////////////////////////////////
// UGetFriendsCallbackProxy
DEFINE_LOG_CATEGORY(ChineseGetFriendsLog);

UGetFriendsCallbackProxy::UGetFriendsCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, FriendListReadCompleteDelegate(FOnReadFriendsListComplete::CreateUObject(this, &ThisClass::OnReadFriendsListCompleted))
{
}

UGetFriendsCallbackProxy* UGetFriendsCallbackProxy::GetAndStoreFriendsList(UObject* WorldContextObject, class APlayerController* PlayerController)
{
	UGetFriendsCallbackProxy* Proxy = NewObject<UGetFriendsCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UGetFriendsCallbackProxy::Activate()
{
	if (!PlayerControllerWeakPtr.IsValid())
	{
		// Fail immediately
		UE_LOG(ChineseGetFriendsLog, Warning, TEXT("获取好友失败，收到了一个错误的玩家控制器！"));
		TArray<FBPFriendInfo> EmptyArray;
		OnFailure.Broadcast(EmptyArray);
		return;
	}

	IOnlineFriendsPtr Friends = Online::GetFriendsInterface(GetWorld());
	if (Friends.IsValid())
	{	
		ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerControllerWeakPtr->Player);

		Friends->ReadFriendsList(Player->GetControllerId(), EFriendsLists::ToString((EFriendsLists::Default)), FriendListReadCompleteDelegate);
		return;
	}

	// Fail immediately
	TArray<FBPFriendInfo> EmptyArray;

	OnFailure.Broadcast(EmptyArray);
}

void UGetFriendsCallbackProxy::OnReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorString)
{
	if (bWasSuccessful)
	{
		IOnlineFriendsPtr Friends = Online::GetFriendsInterface(GetWorld());
		if (Friends.IsValid())
		{
			// 实际上不再需要了，而且没有经过验证，导致崩溃。
			//ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerControllerWeakPtr->Player);

			TArray<FBPFriendInfo> FriendsListOut;
			TArray< TSharedRef<FOnlineFriend> > FriendList;
			Friends->GetFriendsList(LocalUserNum, ListName, FriendList);

			for (int32 i = 0; i < FriendList.Num(); i++)
			{
				TSharedRef<FOnlineFriend> Friend = FriendList[i];
				FBPFriendInfo BPF;
				FOnlineUserPresence pres = Friend->GetPresence();
				BPF.OnlineState = ((EBPOnlinePresenceState)((int32)pres.Status.State));
				BPF.DisplayName = Friend->GetDisplayName();
				BPF.RealName = Friend->GetRealName();
				BPF.UniqueNetId.SetUniqueNetId(Friend->GetUserId());
				BPF.bIsPlayingSameGame = pres.bIsPlayingThisGame;

				BPF.PresenceInfo.bIsOnline = pres.bIsOnline;
				BPF.PresenceInfo.bHasVoiceSupport = pres.bHasVoiceSupport;
				BPF.PresenceInfo.bIsPlaying = pres.bIsPlaying;
				BPF.PresenceInfo.PresenceState = ((EBPOnlinePresenceState)((int32)pres.Status.State));
				BPF.PresenceInfo.StatusString = pres.Status.StatusStr;
				BPF.PresenceInfo.bIsJoinable = pres.bIsJoinable;
				BPF.PresenceInfo.bIsPlayingThisGame = pres.bIsPlayingThisGame;


				FriendsListOut.Add(BPF);
			}

			OnSuccess.Broadcast(FriendsListOut);
		}
	}
	else
	{
		TArray<FBPFriendInfo> EmptyArray;
		OnFailure.Broadcast(EmptyArray);
	}
}
