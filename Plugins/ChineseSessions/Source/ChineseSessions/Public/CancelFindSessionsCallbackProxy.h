//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BlueprintDataDefinitions.h"
#include "CancelFindSessionsCallbackProxy.generated.h"

UCLASS(MinimalAPI)
class UCancelFindSessionsCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当成功销毁时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FEmptyOnlineDelegate OnSuccess;

	// 当销毁失败时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FEmptyOnlineDelegate OnFailure;

	// 取消查找会话
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category = "中文会话|在线|会话" , DisplayName = "取消查找中文会话")
	static UCancelFindSessionsCallbackProxy* CancelFindSessions(UObject* WorldContextObject, UPARAM(DisplayName = "玩家控制器") class APlayerController* PlayerController);

	// UOnlineBlueprintCallProxyBase接口
	virtual void Activate() override;
	// UOnlineBlueprintCallProxyBase接口结束

private:
	// 当操作完成时的内部回调，调用公共的成功/失败回调
	void OnCompleted(bool bWasSuccessful);

private:
	// 触发事物的玩家控制器
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// 由在线子系统执行的委托
	FOnCancelFindSessionsCompleteDelegate Delegate;

	// 已注册的OnDestroySessionComplete委托的句柄
	FDelegateHandle DelegateHandle;

	// 此次调用正在进行的全球上下文对象
	UObject* WorldContextObject;
};
