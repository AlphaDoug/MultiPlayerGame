//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "CreateSessionCallbackProxyChinese.h"


//////////////////////////////////////////////////////////////////////////
// UCreateSessionCallbackProxyChinese

UCreateSessionCallbackProxyChinese::UCreateSessionCallbackProxyChinese(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CreateCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted))
	, StartCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted))
	, NumPublicConnections(1)
{
}

UCreateSessionCallbackProxyChinese* UCreateSessionCallbackProxyChinese::CreateChineseSession(UObject* WorldContextObject, const TArray<FSessionPropertyKeyPair>& ExtraSettings, class APlayerController* PlayerController, int32 PublicConnections, int32 PrivateConnections, bool bUseLAN, bool bAllowInvites, bool bIsDedicatedServer, bool bUsePresence, bool bUseLobbiesIfAvailable, bool bAllowJoinViaPresence, bool bAllowJoinViaPresenceFriendsOnly, bool bAntiCheatProtected, bool bUsesStats, bool bShouldAdvertise, bool bUseLobbiesVoiceChatIfAvailable, bool bStartAfterCreate)
{
	UCreateSessionCallbackProxyChinese* Proxy = NewObject<UCreateSessionCallbackProxyChinese>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->NumPublicConnections = PublicConnections;
	Proxy->NumPrivateConnections = PrivateConnections;
	Proxy->bUseLAN = bUseLAN;
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->bAllowInvites = bAllowInvites;
	Proxy->ExtraSettings = ExtraSettings;
	Proxy->bDedicatedServer = bIsDedicatedServer;
	Proxy->bUsePresence = bUsePresence;
	Proxy->bUseLobbiesIfAvailable = bUseLobbiesIfAvailable;
	Proxy->bAllowJoinViaPresence = bAllowJoinViaPresence;
	Proxy->bAllowJoinViaPresenceFriendsOnly = bAllowJoinViaPresenceFriendsOnly;
	Proxy->bAntiCheatProtected = bAntiCheatProtected;
	Proxy->bUsesStats = bUsesStats;
	Proxy->bShouldAdvertise = bShouldAdvertise;
	Proxy->bUseLobbiesVoiceChatIfAvailable = bUseLobbiesVoiceChatIfAvailable;
	Proxy->bStartAfterCreate = bStartAfterCreate;
	return Proxy;
}

void UCreateSessionCallbackProxyChinese::Activate()
{
	FOnlineSubsystemBPCallHelperChinese Helper(TEXT("创建会话"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));
	
	if (PlayerControllerWeakPtr.IsValid() )
		Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.OnlineSub != nullptr)
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			CreateCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);
			
			FOnlineSessionSettings Settings;
			Settings.NumPublicConnections = NumPublicConnections;
			Settings.NumPrivateConnections = NumPrivateConnections;
			Settings.bShouldAdvertise = bShouldAdvertise;
			Settings.bAllowJoinInProgress = true;
			Settings.bIsLANMatch = bUseLAN;
			Settings.bAllowJoinViaPresence = bAllowJoinViaPresence;
			Settings.bIsDedicated = bDedicatedServer;

			if (bDedicatedServer)
			{
				Settings.bUsesPresence = false;
				Settings.bUseLobbiesIfAvailable = false;
			}
			else
			{
				Settings.bUsesPresence = bUsePresence;
				Settings.bUseLobbiesIfAvailable = bUseLobbiesIfAvailable;
			}

			Settings.bUseLobbiesVoiceChatIfAvailable = bUseLobbiesIfAvailable ? bUseLobbiesVoiceChatIfAvailable : false;
			Settings.bAllowJoinViaPresenceFriendsOnly = bAllowJoinViaPresenceFriendsOnly;
			Settings.bAntiCheatProtected = bAntiCheatProtected;
			Settings.bUsesStats = bUsesStats;

			// These are about the only changes over the standard Create Sessions Node
			Settings.bAllowInvites = bAllowInvites;
			
			FOnlineSessionSetting ExtraSetting;
			for (int i = 0; i < ExtraSettings.Num(); i++)
			{
				ExtraSetting.Data = ExtraSettings[i].Data;
				//	ViaOnlineServiceAndPing
				ExtraSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Settings.Settings.Add(ExtraSettings[i].Key, ExtraSetting);
			}
			
			
			if (!bDedicatedServer )
			{
				if (PlayerControllerWeakPtr.IsValid() && Helper.UserID.IsValid())
				{
					Sessions->CreateSession(*Helper.UserID, NAME_GameSession, Settings);
				}
				else
				{
					FFrame::KismetExecutionMessage(TEXT("尝试开始会话时的玩家控制器无效"), ELogVerbosity::Warning);
					Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);
					
					// Fail immediately
					OnFailure.Broadcast();
				}
			}
			else
				Sessions->CreateSession(0, NAME_GameSession, Settings);

			// OnCreateCompleted will get called, nothing more to do now
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("在线子系统不支持会话"), ELogVerbosity::Warning);
		}
	}

	// Fail immediately
	OnFailure.Broadcast();
}

void UCreateSessionCallbackProxyChinese::OnCreateCompleted(FName SessionName, bool bWasSuccessful)
{
	FOnlineSubsystemBPCallHelperChinese Helper(TEXT("创建会话回调"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));
	//Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.OnlineSub != nullptr)
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);
			
			if (bWasSuccessful)
			{
				if (this->bStartAfterCreate)
				{
					UE_LOG_ONLINE_SESSION(Display, TEXT("会话创建完成。自动开始已打开，现在开始会话。"));
					StartCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(StartCompleteDelegate);
					Sessions->StartSession(NAME_GameSession); // We'll call `OnSuccess.Broadcast()` when start succeeds.
				}
				else
				{
					UE_LOG_ONLINE_SESSION(Display, TEXT("会话创建完成。自动开始已关闭，要开始会话，请调用“开始中文会话”。"));
					OnSuccess.Broadcast();
				}

				// OnStartCompleted will get called, nothing more to do now
				return;
			}
		}
	}

	if (!bWasSuccessful)
	{
		OnFailure.Broadcast();
	}
}

void UCreateSessionCallbackProxyChinese::OnStartCompleted(FName SessionName, bool bWasSuccessful)
{
	FOnlineSubsystemBPCallHelperChinese Helper(TEXT("StartSessionCallback"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));
	//Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.OnlineSub != nullptr)
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(StartCompleteDelegateHandle);
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
