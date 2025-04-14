//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "StartSessionCallbackProxyChinese.h"

UStartSessionCallbackProxyChinese::UStartSessionCallbackProxyChinese(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , StartCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted))
{
}

UStartSessionCallbackProxyChinese* UStartSessionCallbackProxyChinese::StartChineseSession(
	const UObject* WorldContextObject)
{
	UStartSessionCallbackProxyChinese* Proxy = NewObject<UStartSessionCallbackProxyChinese>();
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UStartSessionCallbackProxyChinese::Activate()
{
	const FOnlineSubsystemBPCallHelperChinese Helper(
		TEXT("开始会话"),
		GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));

	if (Helper.OnlineSub != nullptr)
	{
		const auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			StartCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(StartCompleteDelegate);
			Sessions->StartSession(NAME_GameSession);
			return;
		}
		FFrame::KismetExecutionMessage(TEXT("在线子系统不支持会话"), ELogVerbosity::Warning);
	}

	// Fail immediately
	OnFailure.Broadcast();
}

void UStartSessionCallbackProxyChinese::OnStartCompleted(FName SessionName, bool bWasSuccessful)
{
	const FOnlineSubsystemBPCallHelperChinese Helper(
		TEXT("启动会话回调"),
		GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull));

	if (Helper.OnlineSub != nullptr)
	{
		const auto Sessions = Helper.OnlineSub->GetSessionInterface();
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