//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023


#include "ChineseFriendsLibrary.h"



// This is taken directly from UE4 - OnlineSubsystemSteamPrivatePCH.h as a fix for the array_count macro

//General Log
DEFINE_LOG_CATEGORY(ChineseFriendsLog);

void UChineseFriendsLibrary::SendSessionInviteToFriends(APlayerController *PlayerController, const TArray<FBPUniqueNetId> &Friends, EBlueprintResultSwitch &Result)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请： 有一个坏的玩家控制器！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (Friends.Num() < 1)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请：  邀请数组中没有朋友！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface();

	if (!SessionInterface.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请：  无法获取会话接口！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerController->Player);

	if (!Player)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请：  无法获取本地玩家！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	TArray<TSharedRef<const FUniqueNetId>> List;
	for (int i = 0; i < Friends.Num(); i++)
	{
		TSharedRef<const FUniqueNetId> val(Friends[i].UniqueNetId.ToSharedRef());
		//TSharedRef<const FUniqueNetId> val(Friends[i].GetUniqueNetId());
		List.Add(val);
	}

	if (SessionInterface->SendSessionInviteToFriends(Player->GetControllerId(), NAME_GameSession, List))
	{
		Result = EBlueprintResultSwitch::OnSuccess;
		return;
	}

	Result = EBlueprintResultSwitch::OnFailure;
	return;
}

void UChineseFriendsLibrary::SendSessionInviteToFriend(APlayerController *PlayerController, const FBPUniqueNetId &FriendUniqueNetId, EBlueprintResultSwitch &Result)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请： 遇到了一个错误的玩家控制器！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (!FriendUniqueNetId.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请： 遇到了一个错误的网络唯一ID!"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface();

	if (!SessionInterface.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请： 无法获取会话接口！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerController->Player);

	if (!Player)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("向好友发送会话邀请： 无法获取本地玩家"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (SessionInterface->SendSessionInviteToFriend(Player->GetControllerId(), NAME_GameSession, *FriendUniqueNetId.GetUniqueNetId()))
	{
		Result = EBlueprintResultSwitch::OnSuccess;
		return;
	}

	Result = EBlueprintResultSwitch::OnFailure;
	return;
}

void UChineseFriendsLibrary::GetFriend(APlayerController *PlayerController, const FBPUniqueNetId FriendUniqueNetId, FBPFriendInfo &Friend)
{

	if (!PlayerController)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取好友： 遇到了一个有问题的玩家控制器！"));
		return;
	}

	if (!FriendUniqueNetId.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取好友： 遇到了一个有问题的唯一网络ID！"));
		return;
	}

	IOnlineFriendsPtr FriendsInterface = Online::GetFriendsInterface();

	if (!FriendsInterface.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取好友： 获取好友接口失败！"));
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerController->Player);

	if (!Player)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取好友： 获取本地玩家失败！"));
		return;
	}

	TSharedPtr<FOnlineFriend> fr = FriendsInterface->GetFriend(Player->GetControllerId(), *FriendUniqueNetId.GetUniqueNetId(), EFriendsLists::ToString(EFriendsLists::Default));
	if (fr.IsValid())
	{
		FOnlineUserPresence pres = fr->GetPresence();
		Friend.DisplayName = fr->GetDisplayName();
		Friend.OnlineState = ((EBPOnlinePresenceState)((int32)pres.Status.State));
		Friend.RealName = fr->GetRealName();
		Friend.UniqueNetId.SetUniqueNetId(fr->GetUserId());
		Friend.bIsPlayingSameGame = pres.bIsPlayingThisGame;

		Friend.PresenceInfo.bHasVoiceSupport = pres.bHasVoiceSupport;
		Friend.PresenceInfo.bIsJoinable = pres.bIsJoinable;
		Friend.PresenceInfo.bIsOnline = pres.bIsOnline;
		Friend.PresenceInfo.bIsPlaying = pres.bIsPlaying;
		Friend.PresenceInfo.bIsPlayingThisGame = pres.bIsPlayingThisGame;
		Friend.PresenceInfo.PresenceState = ((EBPOnlinePresenceState)((int32)pres.Status.State));
		Friend.PresenceInfo.StatusString = pres.Status.StatusStr;
	}
}

void UChineseFriendsLibrary::IsAFriend(APlayerController *PlayerController, const FBPUniqueNetId UniqueNetId, bool &IsFriend)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("是好友： 遇到了一个有问题的玩家控制器！"));
		return;
	}

	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("是好友： 遇到了一个有问题的唯一网络ID！"));
		return;
	}

	IOnlineFriendsPtr FriendsInterface = Online::GetFriendsInterface();

	if (!FriendsInterface.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("是好友： 获取好友接口失败！"));
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerController->Player);

	if (!Player)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("是好友： F获取本地玩家失败！"));
		return;
	}

	IsFriend = FriendsInterface->IsFriend(Player->GetControllerId(), *UniqueNetId.GetUniqueNetId(), EFriendsLists::ToString(EFriendsLists::Default));
}

void UChineseFriendsLibrary::GetStoredRecentPlayersList(FBPUniqueNetId UniqueNetId, TArray<FBPOnlineRecentPlayer> &PlayersList)
{
	IOnlineFriendsPtr FriendsInterface = Online::GetFriendsInterface();
	
	if (!FriendsInterface.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取最近的玩家列表： 获取好友接口失败！"));
		return;
	}

	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取最近的玩家列表： 给出了一个无效的唯一网络ID！"));
		return;
	}

	TArray< TSharedRef<FOnlineRecentPlayer> > PlayerList;
	
	// For now getting all namespaces
	FriendsInterface->GetRecentPlayers(*(UniqueNetId.GetUniqueNetId()),"", PlayerList);

	for (int32 i = 0; i < PlayerList.Num(); i++)
	{
		TSharedRef<FOnlineRecentPlayer> Player = PlayerList[i];
		FBPOnlineRecentPlayer BPF;
		BPF.DisplayName = Player->GetDisplayName();
		BPF.RealName = Player->GetRealName();
		BPF.UniqueNetId.SetUniqueNetId(Player->GetUserId());
		PlayersList.Add(BPF);
	}
}

void UChineseFriendsLibrary::GetStoredFriendsList(APlayerController *PlayerController, TArray<FBPFriendInfo> &FriendsList)
{

	if (!PlayerController)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取好友们列表： 遇到了一个有问题的玩家控制器！"));
		return;
	}

	IOnlineFriendsPtr FriendsInterface = Online::GetFriendsInterface();
	
	if (!FriendsInterface.IsValid())
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取好友们列表： 获取好友接口失败！"));
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerController->Player);

	if (!Player)
	{
		UE_LOG(ChineseFriendsLog, Warning, TEXT("获取好友们列表： 获取本地玩家失败！"));
		return;
	}


	TArray< TSharedRef<FOnlineFriend> > FriendList;
	FriendsInterface->GetFriendsList(Player->GetControllerId(), EFriendsLists::ToString((EFriendsLists::Default)), FriendList);

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

		FriendsList.Add(BPF);
	}
}