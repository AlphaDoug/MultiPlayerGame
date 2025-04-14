//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Engine/LocalPlayer.h"
#include "LogoutUserCallbackProxy.generated.h"

UCLASS(MinimalAPI)
class ULogoutUserCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当成功销毁时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FEmptyOnlineDelegate OnSuccess;

	// 当销毁失败时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FEmptyOnlineDelegate OnFailure;

	// 从身份接口注销
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject", DisplayName = "注销用户"), Category = "中文会话|在线|身份")
	static ULogoutUserCallbackProxy* LogoutUser(UObject* WorldContextObject, UPARAM(DisplayName = "玩家控制器") class APlayerController* PlayerController);

	// UOnlineBlueprintCallProxyBase接口
	virtual void Activate() override;
	// UOnlineBlueprintCallProxyBase接口结束

private:
	// 操作完成时的内部回调，调用公共成功/失败回调
	void OnCompleted(int LocalUserNum, bool bWasSuccessful);

private:
	// 触发事情的玩家控制器
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// 在线子系统执行的委托
	FOnLogoutCompleteDelegate Delegate;

	// 注册的OnDestroySessionComplete委托的句柄
	FDelegateHandle DelegateHandle;

	// 进行此调用的世界上下文对象
	UObject* WorldContextObject;
};
