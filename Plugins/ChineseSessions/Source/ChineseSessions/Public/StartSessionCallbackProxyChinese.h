//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "StartSessionCallbackProxyChinese.generated.h"

UCLASS(MinimalAPI)
class UStartSessionCallbackProxyChinese : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()
	// Called when the session starts successfully
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	// Called when there is an error starting the session
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	/**
	 * 使用默认的在线子系统启动一个会话。该会话需要先通过调用“创建中文会话”节点来创建。
	 * @param WorldContextObject
	 */
	UFUNCTION(
		BlueprintCallable
		, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject", DisplayName = "开始中文会话 ")
		, Category = "中文会话|在线|会话"
	)
	static UStartSessionCallbackProxyChinese* StartChineseSession(const UObject* WorldContextObject);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// 当会话开始完成时的内部回调
	void OnStartCompleted(FName SessionName, bool bWasSuccessful);

	// 在线子系统执行的委托
	FOnStartSessionCompleteDelegate StartCompleteDelegate;

	// 上述注册的委托的句柄
	FDelegateHandle StartCompleteDelegateHandle;

	// 进行此次调用的世界上下文对象
	const UObject* WorldContextObject;

};