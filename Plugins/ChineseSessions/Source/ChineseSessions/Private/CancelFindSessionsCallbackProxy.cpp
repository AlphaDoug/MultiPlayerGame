//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "CancelFindSessionsCallbackProxy.h"


//////////////////////////////////////////////////////////////////////////
// UCancelFindSessionsCallbackProxy

UCancelFindSessionsCallbackProxy::UCancelFindSessionsCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnCancelFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
{
}

UCancelFindSessionsCallbackProxy* UCancelFindSessionsCallbackProxy::CancelFindSessions(UObject* WorldContextObject, class APlayerController* PlayerController)
{
	UCancelFindSessionsCallbackProxy* Proxy = NewObject<UCancelFindSessionsCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UCancelFindSessionsCallbackProxy::Activate()
{
	FOnlineSubsystemBPCallHelperChinese Helper(TEXT("取消查找会话"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())		
		{
			DelegateHandle = Sessions->AddOnCancelFindSessionsCompleteDelegate_Handle(Delegate);
			Sessions->CancelFindSessions();

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

void UCancelFindSessionsCallbackProxy::OnCompleted(bool bWasSuccessful)
{
	FOnlineSubsystemBPCallHelperChinese Helper(TEXT("取消查找会话回调"), GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnCancelFindSessionsCompleteDelegate_Handle(DelegateHandle);
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
