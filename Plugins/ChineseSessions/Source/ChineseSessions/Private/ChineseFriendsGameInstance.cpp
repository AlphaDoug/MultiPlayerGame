//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "ChineseFriendsGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

//General Log
	DEFINE_LOG_CATEGORY(ChineseFriendsInterfaceLog);

UChineseFriendsGameInstance::UChineseFriendsGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bCallFriendInterfaceEventsOnPlayerControllers(true)
	, bCallIdentityInterfaceEventsOnPlayerControllers(true)
	, bCallVoiceInterfaceEventsOnPlayerControllers(true)
	, bEnableTalkingStatusDelegate(true)
	, SessionInviteReceivedDelegate(FOnSessionInviteReceivedDelegate::CreateUObject(this, &ThisClass::OnSessionInviteReceivedMaster))
	, SessionInviteAcceptedDelegate(FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &ThisClass::OnSessionInviteAcceptedMaster))
	, PlayerTalkingStateChangedDelegate(FOnPlayerTalkingStateChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerTalkingStateChangedMaster))
	, PlayerLoginChangedDelegate(FOnLoginChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerLoginChangedMaster))
	, PlayerLoginStatusChangedDelegate(FOnLoginStatusChangedDelegate::CreateUObject(this, &ThisClass::OnPlayerLoginStatusChangedMaster))
{
}

void UChineseFriendsGameInstance::Shutdown()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	
	if (!SessionInterface.IsValid())
	{
		UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：获取会话系统失败！"));
		//return;
	}
	else
	{
		// 在这里清除所有的委托句柄
		SessionInterface->ClearOnSessionUserInviteAcceptedDelegate_Handle(SessionInviteAcceptedDelegateHandle);
		SessionInterface->ClearOnSessionInviteReceivedDelegate_Handle(SessionInviteReceivedDelegateHandle);
	}


	if (bEnableTalkingStatusDelegate)
	{
		IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(GetWorld());

		if (VoiceInterface.IsValid())
		{
			VoiceInterface->ClearOnPlayerTalkingStateChangedDelegate_Handle(PlayerTalkingStateChangedDelegateHandle);
		}
		else
		{

			UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：获取语音接口失败！"));
		}
	}

	IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld());

	if (IdentityInterface.IsValid())
	{
		IdentityInterface->ClearOnLoginChangedDelegate_Handle(PlayerLoginChangedDelegateHandle);
		

		// 默认为玩家1
		IdentityInterface->ClearOnLoginStatusChangedDelegate_Handle(0, PlayerLoginStatusChangedDelegateHandle);
	}


	Super::Shutdown();
}

void UChineseFriendsGameInstance::Init()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());//OnlineSub->GetSessionInterface();

	if (SessionInterface.IsValid())
	{
		// 目前并没有为第一个之外的任何本地玩家存储句柄或分配委托…应该处理吗？
		// 考虑过直接处理它，但是多个玩家的朋友可能并不需要
		// 遍历本地玩家TArray只有在为其分配了玩家时才有效，大多数在线接口也不支持
		// 多次登录（例如：Steam）
		SessionInviteAcceptedDelegateHandle = SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(SessionInviteAcceptedDelegate);

		SessionInviteReceivedDelegateHandle = SessionInterface->AddOnSessionInviteReceivedDelegate_Handle(SessionInviteReceivedDelegate);
	}
	else
	{
		UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：获取会话接口失败"));
		//return;
	}

	// 开始在语音接口上工作
	if (bEnableTalkingStatusDelegate)
	{
		IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(GetWorld());

		if (VoiceInterface.IsValid())
		{
			PlayerTalkingStateChangedDelegateHandle = VoiceInterface->AddOnPlayerTalkingStateChangedDelegate_Handle(PlayerTalkingStateChangedDelegate);
		}
		else
		{

			UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：获取语音接口失败！"));
		}
	}

	IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld());

	if (IdentityInterface.IsValid())
	{
		PlayerLoginChangedDelegateHandle = IdentityInterface->AddOnLoginChangedDelegate_Handle(PlayerLoginChangedDelegate);

		// Just defaulting to player 1
		PlayerLoginStatusChangedDelegateHandle = IdentityInterface->AddOnLoginStatusChangedDelegate_Handle(0, PlayerLoginStatusChangedDelegate);
	}
	else
	{
		UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：获取身份接口失败！"));
	}


	Super::Init();
}


void UChineseFriendsGameInstance::OnPlayerLoginStatusChangedMaster(int32 PlayerNum, ELoginStatus::Type PreviousStatus, ELoginStatus::Type NewStatus, const FUniqueNetId & NewPlayerUniqueNetID)
{
	EBPLoginStatus OrigStatus = (EBPLoginStatus)PreviousStatus;
	EBPLoginStatus CurrentStatus = (EBPLoginStatus)NewStatus;
	FBPUniqueNetId PlayerID;
	PlayerID.SetUniqueNetId(&NewPlayerUniqueNetID);

	OnPlayerLoginStatusChanged(PlayerNum, OrigStatus,CurrentStatus,PlayerID);


	if (bCallIdentityInterfaceEventsOnPlayerControllers)
	{
		APlayerController* Player = UGameplayStatics::GetPlayerController(GetWorld(), PlayerNum);

		if (Player != NULL)
		{
			//Run the Event specific to the actor, if the actor has the interface, otherwise ignore
			if (Player->GetClass()->ImplementsInterface(UChineseFriendsInterface::StaticClass()))
			{
				IChineseFriendsInterface::Execute_OnPlayerLoginStatusChanged(Player, OrigStatus, CurrentStatus, PlayerID);
			}
		}
		else
		{
			UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：在“玩家登录状态更改主机”中无法获取指定索引的控制器！"));
		}
	}
}

void UChineseFriendsGameInstance::OnPlayerLoginChangedMaster(int32 PlayerNum)
{
	OnPlayerLoginChanged(PlayerNum);

	if (bCallIdentityInterfaceEventsOnPlayerControllers)
	{
		APlayerController* Player = UGameplayStatics::GetPlayerController(GetWorld(), PlayerNum);

		if (Player != NULL)
		{
			//Run the Event specific to the actor, if the actor has the interface, otherwise ignore
			if (Player->GetClass()->ImplementsInterface(UChineseFriendsInterface::StaticClass()))
			{
				IChineseFriendsInterface::Execute_OnPlayerLoginChanged(Player, PlayerNum);
			}
		}
		else
		{
			UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：在“玩家登录更改”中无法获取指定索引的控制器！"));
		}
	}
}

void UChineseFriendsGameInstance::OnPlayerTalkingStateChangedMaster(TSharedRef<const FUniqueNetId> PlayerId, bool bIsTalking)
{
	FBPUniqueNetId PlayerTalking;
	PlayerTalking.SetUniqueNetId(PlayerId);
	OnPlayerTalkingStateChanged(PlayerTalking, bIsTalking);

	if (bCallVoiceInterfaceEventsOnPlayerControllers)
	{
		APlayerController* Player = NULL;

		for (const ULocalPlayer* LPlayer : LocalPlayers)
		{
			Player = UGameplayStatics::GetPlayerController(GetWorld(), LPlayer->GetControllerId());

			if (Player != NULL)
			{
				//Run the Event specific to the actor, if the actor has the interface, otherwise ignore
				if (Player->GetClass()->ImplementsInterface(UChineseFriendsInterface::StaticClass()))
				{
					IChineseFriendsInterface::Execute_OnPlayerVoiceStateChanged(Player, PlayerTalking, bIsTalking);
				}
			}
			else
			{
				UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：在“语音状态更改时”中无法获取指定索引的控制器！"));
			}
		}
	}
}

void UChineseFriendsGameInstance::OnSessionInviteReceivedMaster(const FUniqueNetId & PersonInvited, const FUniqueNetId & PersonInviting, const FString& AppId, const FOnlineSessionSearchResult& SessionToJoin)
{
	if (SessionToJoin.IsValid())
	{
		FBlueprintSessionResult BluePrintResult;
		BluePrintResult.OnlineResult = SessionToJoin;

		FBPUniqueNetId PInvited;
		PInvited.SetUniqueNetId(&PersonInvited);

		FBPUniqueNetId PInviting;
		PInviting.SetUniqueNetId(&PersonInviting);


		TArray<APlayerController*> PlayerList;
		GEngine->GetAllLocalPlayerControllers(PlayerList);

		APlayerController* Player = NULL;

		int32 LocalPlayer = 0;
		for (int i = 0; i < PlayerList.Num(); i++)
		{
			if (*PlayerList[i]->PlayerState->GetUniqueId().GetUniqueNetId() == PersonInvited)
			{
				LocalPlayer = i;
				Player = PlayerList[i];
				break;
			}
		}

		OnSessionInviteReceived(LocalPlayer, PInviting, AppId, BluePrintResult);

		//IAdvancedFriendsInterface* TheInterface = NULL;

		if (Player != NULL)
		{
			//Run the Event specific to the actor, if the actor has the interface, otherwise ignore
			if (Player->GetClass()->ImplementsInterface(UChineseFriendsInterface::StaticClass()))
			{
				IChineseFriendsInterface::Execute_OnSessionInviteReceived(Player, PInviting, BluePrintResult);
			}
		}
		else
		{
			UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：在“收到会话邀请时”中无法获取指定索引的控制器！"));
		}
	}
	else
	{
		UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：在“收到会话邀请时”中返回了一个错误的搜索结果！"));
	}
}

void UChineseFriendsGameInstance::OnSessionInviteAcceptedMaster(const bool bWasSuccessful, int32 LocalPlayer, TSharedPtr<const FUniqueNetId> PersonInvited, const FOnlineSessionSearchResult& SessionToJoin)
{
	if (bWasSuccessful)
	{
		if (SessionToJoin.IsValid())
		{

			FBlueprintSessionResult BluePrintResult;
			BluePrintResult.OnlineResult = SessionToJoin;

			FBPUniqueNetId PInvited;
			PInvited.SetUniqueNetId(PersonInvited);

			OnSessionInviteAccepted(LocalPlayer,PInvited, BluePrintResult);

			APlayerController* Player = UGameplayStatics::GetPlayerController(GetWorld(), LocalPlayer);

			//IChineseFriendsInterface* TheInterface = NULL;

			if (Player != NULL)
			{
				//如果角色具有接口，则运行特定于角色的事件，否则忽略
				if (Player->GetClass()->ImplementsInterface(UChineseFriendsInterface::StaticClass()))
				{
					IChineseFriendsInterface::Execute_OnSessionInviteAccepted(Player,PInvited, BluePrintResult);
				}
			}
			else
			{ 
				UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：在“已接受会话邀请”中无法获取指定索引的控制器！"));
			}
		}
		else
		{
			UE_LOG(ChineseFriendsInterfaceLog, Warning, TEXT("中文好友游戏实例：在“已接受会话邀请”中返回了一个错误的搜索结果！"));
		}
	}
}