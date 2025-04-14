//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BlueprintDataDefinitions.h"
#include "EndSessionCallbackProxy.generated.h"

UCLASS(MinimalAPI)
class UEndSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// Called when there is a successful destroy
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FEmptyOnlineDelegate OnSuccess;

	// Called when there is an unsuccessful destroy
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FEmptyOnlineDelegate OnFailure;

	/**
	 *    结束当前会话，通常在几乎所有用途中，你应该使用引擎的原生“销毁会话”节点。
	 *    这个存在是为了使用“启动会话”的人，并且可选地手动管理会话状态。
	 */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject", DisplayName = "结束中文会话（非销毁）"), Category = "中文会话|在线|会话")
	static UEndSessionCallbackProxy* EndSession(UObject* WorldContextObject, UPARAM(DisplayName = "玩家控制器") class APlayerController* PlayerController);

	// UOnlineBlueprintCallProxyBase 接口
	virtual void Activate() override;
	// UOnlineBlueprintCallProxyBase 接口结束

private:
	// 当操作完成时的内部回调，调用公共的成功/失败回调。
	void OnCompleted(FName SessionName, bool bWasSuccessful);

private:
	// 玩家控制器触发事物
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// 在线子系统执行的委托
	FOnEndSessionCompleteDelegate Delegate;

	// 注册的OnDestroySessionComplete委托的句柄
	FDelegateHandle DelegateHandle;

	// 这个调用发生的世界上下文对象
	UObject* WorldContextObject;
};
