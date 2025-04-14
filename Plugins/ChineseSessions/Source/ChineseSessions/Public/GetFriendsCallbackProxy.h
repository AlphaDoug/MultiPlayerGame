//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Engine/LocalPlayer.h"
#include "GetFriendsCallbackProxy.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ChineseGetFriendsLog, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintGetFriendsListDelegate, const TArray<FBPFriendInfo>&, Results);

UCLASS(MinimalAPI)
class UGetFriendsCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当成功获取好友列表时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FBlueprintGetFriendsListDelegate OnSuccess;

	// 在获取好友列表出错时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FBlueprintGetFriendsListDelegate OnFailure;

	// 从OnlineSubsystem获取玩家的好友列表并返回，可以稍后通过“获取存储的好友列表”获取
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject", DisplayName = "获取并存储好友列表"), Category = "中文会话|在线|好友")
	static UGetFriendsCallbackProxy* GetAndStoreFriendsList(UObject* WorldContextObject, UPARAM(DisplayName = "玩家控制器") class APlayerController* PlayerController);

	virtual void Activate() override;

private:
	// 当获取好友列表时的内部回调 
	void OnReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorString);

	// 触发事件的玩家控制器
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// 执行的委托
	FOnReadFriendsListComplete FriendListReadCompleteDelegate;


	// 进行此调用的世界上下文对象
	UObject* WorldContextObject;
};

