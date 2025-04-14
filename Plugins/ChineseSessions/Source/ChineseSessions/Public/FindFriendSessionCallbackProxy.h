//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Engine/LocalPlayer.h"
#include "FindFriendSessionCallbackProxy.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ChineseFindFriendSessionLog, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintFindFriendSessionDelegate, const TArray<FBlueprintSessionResult> &, SessionInfo);

UCLASS(MinimalAPI)
class UFindFriendSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当成功获取到好友列表时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FBlueprintFindFriendSessionDelegate OnSuccess;

	// 当获取好友列表出错时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FBlueprintFindFriendSessionDelegate OnFailure;

	// 尝试获取好友当前所在的会话
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject", DisplayName = "查找好友会话"), Category = "中文会话|在线|好友")
	static UFindFriendSessionCallbackProxy* FindFriendSession(UObject* WorldContextObject, UPARAM(DisplayName = "玩家控制器") APlayerController *PlayerController, UPARAM(DisplayName = "好友唯一网络ID") const FBPUniqueNetId &FriendUniqueNetId);

	virtual void Activate() override;

private:
	// 当获取到好友列表时的内部回调
	void OnFindFriendSessionCompleted(int32 LocalPlayer, bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionInfo);

	// 触发事件的玩家控制器
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// 邀请人的唯一网络ID
	FBPUniqueNetId cUniqueNetId;

	// 完成时调用的委托
	FOnFindFriendSessionCompleteDelegate OnFindFriendSessionCompleteDelegate;

	// 上述已注册委托的句柄
	FDelegateHandle FindFriendSessionCompleteDelegateHandle;

	// 进行此次调用的世界上下文对象
	UObject* WorldContextObject;
};

