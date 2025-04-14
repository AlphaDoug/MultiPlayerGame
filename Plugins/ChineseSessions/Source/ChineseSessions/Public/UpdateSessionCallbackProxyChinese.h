//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023


#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "BlueprintDataDefinitions.h"
#include "UpdateSessionCallbackProxyChinese.generated.h"

UCLASS(MinimalAPI)
class UUpdateSessionCallbackProxyChinese : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当会话成功更新时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FEmptyOnlineDelegate OnSuccess;

	// 更新会话出错时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FEmptyOnlineDelegate OnFailure;

	// 使用默认的在线子系统创建会话，并带有高级可选输入，你必须填写所有类别，否则它会传入你不想要的默认值
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject",AutoCreateRefTerm="ExtraSettings"), Category = "中文会话|在线|会话", DisplayName = "更新中文会话")
	static UUpdateSessionCallbackProxyChinese* UpdateSession(UPARAM(DisplayName = "世界上下文对象") UObject* WorldContextObject, UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair> &ExtraSettings, UPARAM(DisplayName = "公开连接") int32 PublicConnections = 100, UPARAM(DisplayName = "私有连接") int32 PrivateConnections = 0, UPARAM(DisplayName = "使用局域网") bool bUseLAN = false, UPARAM(DisplayName = "允许邀请") bool bAllowInvites = false, UPARAM(DisplayName = "允许加入进行中的游戏")  bool bAllowJoinInProgress = false, UPARAM(DisplayName = "是否更新在线数据")  bool bRefreshOnlineData = true, UPARAM(DisplayName = "是否为专用服务器") bool bIsDedicatedServer = false);

	virtual void Activate() override;

private:
	// 会话创建完成时的内部回调，调用“开始会话”
	void OnUpdateCompleted(FName SessionName, bool bWasSuccessful);

	// 在线子系统执行的委托
	FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;

	// 上述已注册委托的句柄
	FDelegateHandle OnUpdateSessionCompleteDelegateHandle;

	// 公开连接的数量
	int NumPublicConnections;

	// 私有连接的数量
	int NumPrivateConnections;

	// 是否搜索局域网
	bool bUseLAN;

	// 是否允许邀请
	bool bAllowInvites;

	// 存储额外设置
	TArray<FSessionPropertyKeyPair> ExtraSettings;

	// 是否更新在线数据
	bool bRefreshOnlineData;

	// 允许加入进行中的游戏
	bool bAllowJoinInProgress;

	// 更新这是否是专用服务器
	bool bDedicatedServer;

	// 进行此次调用的世界上下文对象
	UObject* WorldContextObject;
};

