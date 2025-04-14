//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "EndSessionCallbackProxy.h"


//////////////////////////////////////////////////////////////////////////
// UEndSessionCallbackProxy

UEndSessionCallbackProxy::UEndSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
{
}

UEndSessionCallbackProxy* UEndSessionCallbackProxy::EndSession(UObject* WorldContextObject, class APlayerController* PlayerController)
{
	UEndSessionCallbackProxy* Proxy = NewObject<UEndSessionCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UEndSessionCallbackProxy::Activate()
{
	FOnlineSubsystemBPCallHelperChinese Helper(TEXT("结束会话"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FNamedOnlineSession* Session = Sessions->GetNamedSession(NAME_GameSession);
			if (Session &&
				Session->SessionState == EOnlineSessionState::InProgress)
			{
				DelegateHandle = Sessions->AddOnEndSessionCompleteDelegate_Handle(Delegate);
				Sessions->EndSession(NAME_GameSession);
			}
			else
			{
				OnSuccess.Broadcast();
			}
			// OnCompleted will get called, nothing more to do now
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

void UEndSessionCallbackProxy::OnCompleted(FName SessionName, bool bWasSuccessful)
{
	FOnlineSubsystemBPCallHelperChinese Helper(TEXT("结束会话回调"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnEndSessionCompleteDelegate_Handle(DelegateHandle);
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
