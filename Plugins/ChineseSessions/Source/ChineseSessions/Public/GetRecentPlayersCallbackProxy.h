//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "GetRecentPlayersCallbackProxy.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ChineseGetRecentPlayersLog, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintGetRecentPlayersDelegate, const TArray<FBPOnlineRecentPlayer>&, Results);

UCLASS(MinimalAPI)
class UGetRecentPlayersCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当成功获取好友列表时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FBlueprintGetRecentPlayersDelegate OnSuccess;

	// 在获取好友列表出错时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FBlueprintGetRecentPlayersDelegate OnFailure;

	// 从OnlineSubsystem获取最近的玩家列表并返回，可以稍后通过“获取存储的最近玩家列表”获取，如果没有找到最近的玩家，可能会失败
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject", DisplayName = "获取并存储最近玩家列表"), Category = "中文会话|在线|好友")
	static UGetRecentPlayersCallbackProxy* GetAndStoreRecentPlayersList(UObject* WorldContextObject, UPARAM(DisplayName = "唯一网络ID") const FBPUniqueNetId &UniqueNetId);

	virtual void Activate() override;

private:
	// 当获取好友列表时的内部回调
	void OnQueryRecentPlayersCompleted(const FUniqueNetId &UserID, const FString &Namespace, bool bWasSuccessful, const FString& ErrorString);
	// 已注册的“查找会话完成时”委托的句柄
	FDelegateHandle DelegateHandle;


	// 获取最近玩家的人的唯一网络ID
	FBPUniqueNetId cUniqueNetId;

	// 执行的委托
	FOnQueryRecentPlayersCompleteDelegate QueryRecentPlayersCompleteDelegate;

	// 进行此调用的世界上下文对象
	UObject* WorldContextObject;
};

