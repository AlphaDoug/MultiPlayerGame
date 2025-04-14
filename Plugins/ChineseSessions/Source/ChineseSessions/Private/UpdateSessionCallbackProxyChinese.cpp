//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023


#include "UpdateSessionCallbackProxyChinese.h"


//////////////////////////////////////////////////////////////////////////
// UUpdateSessionCallbackProxyChinese

UUpdateSessionCallbackProxyChinese::UUpdateSessionCallbackProxyChinese(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, OnUpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateCompleted))
	, NumPublicConnections(1)
{
}	

UUpdateSessionCallbackProxyChinese* UUpdateSessionCallbackProxyChinese::UpdateSession(UObject* WorldContextObject, const TArray<FSessionPropertyKeyPair> &ExtraSettings, int32 PublicConnections, int32 PrivateConnections, bool bUseLAN, bool bAllowInvites, bool bAllowJoinInProgress, bool bRefreshOnlineData, bool bIsDedicatedServer)
{
	UUpdateSessionCallbackProxyChinese* Proxy = NewObject<UUpdateSessionCallbackProxyChinese>();
	Proxy->NumPublicConnections = PublicConnections;
	Proxy->NumPrivateConnections = PrivateConnections;
	Proxy->bUseLAN = bUseLAN;
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->bAllowInvites = bAllowInvites;
	Proxy->ExtraSettings = ExtraSettings;
	Proxy->bRefreshOnlineData = bRefreshOnlineData;
	Proxy->bAllowJoinInProgress = bAllowJoinInProgress;
	Proxy->bDedicatedServer = bIsDedicatedServer;
	return Proxy;	
}

void UUpdateSessionCallbackProxyChinese::Activate()
{
	const FOnlineSubsystemBPCallHelperChinese Helper(TEXT("更新会话"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));

	if (Helper.OnlineSub != nullptr)
	{
		const auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			if (Sessions->GetNumSessions() < 1)
			{
				OnFailure.Broadcast();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("没有注册的会话！"));
				return;
			}

			// This gets the actual session itself
			//FNamedOnlineSession * curSession = Sessions->GetNamedSession(NAME_GameSession);
			FOnlineSessionSettings* Settings = Sessions->GetSessionSettings(NAME_GameSession);

			if (!Settings)
			{
				// Fail immediately
				OnFailure.Broadcast();
				return;
			}

			OnUpdateSessionCompleteDelegateHandle = Sessions->AddOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegate);

		//	FOnlineSessionSettings Settings;
			//Settings->BuildUniqueId = GetBuildUniqueId();
			Settings->NumPublicConnections = NumPublicConnections;
			Settings->NumPrivateConnections = NumPrivateConnections;
			//Settings->bShouldAdvertise = true;
			Settings->bAllowJoinInProgress = bAllowJoinInProgress;
			Settings->bIsLANMatch = bUseLAN;
			//Settings->bUsesPresence = true;
			//Settings->bAllowJoinViaPresence = true;
			Settings->bAllowInvites = bAllowInvites;
			Settings->bAllowJoinInProgress = bAllowJoinInProgress;
			Settings->bIsDedicated = bDedicatedServer;

			FOnlineSessionSetting * fSetting = NULL;
			FOnlineSessionSetting ExtraSetting;
			for (int i = 0; i < ExtraSettings.Num(); i++)
			{
				fSetting = Settings->Settings.Find(ExtraSettings[i].Key);

				if (fSetting)
				{
					fSetting->Data = ExtraSettings[i].Data;
				}
				else
				{
					ExtraSetting.Data = ExtraSettings[i].Data;
					ExtraSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
					Settings->Settings.Add(ExtraSettings[i].Key, ExtraSetting);
				}
			}

			Sessions->UpdateSession(NAME_GameSession, *Settings, bRefreshOnlineData);

			// OnUpdateCompleted will get called, nothing more to do now
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("在线子系统不支持会话"), ELogVerbosity::Warning);
		}
	}
	// Fail immediately
	OnFailure.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("不支持会话"));
}

void UUpdateSessionCallbackProxyChinese::OnUpdateCompleted(FName SessionName, bool bWasSuccessful)
{
	const FOnlineSubsystemBPCallHelperChinese Helper(TEXT("更新会话回调"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));

	if (Helper.OnlineSub != nullptr)
	{
		const auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
				
			if (bWasSuccessful)
			{
				OnSuccess.Broadcast();
				return;
			}
		}
	}

	if (!bWasSuccessful)
	{
		OnFailure.Broadcast();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("没有成功"));
	}
}