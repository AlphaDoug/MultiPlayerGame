//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Engine/LocalPlayer.h"
#include "AutoLoginUserCallbackProxy.generated.h"

UCLASS(MinimalAPI)
class UAutoLoginUserCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当成功销毁时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FEmptyOnlineDelegate OnSuccess;

	// 当销毁失败时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FEmptyOnlineDelegate OnFailure;

	/**
	 * 使用命令行传递的参数将玩家登录到在线服务。期望 -AUTH_LOGIN=<用户名> -AUTH_PASSWORD=<密码>。如果缺少任何一个，
	 * 函数返回false并不启动登录过程
	 *
	 * 本地用户编号:关联用户的控制器编号
	 *
	 */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category = "中文会话|在线|身份" , DisplayName = "自动登录用户")
	static UAutoLoginUserCallbackProxy* AutoLoginUser(UObject* WorldContextObject, UPARAM(DisplayName = "本地用户编号") int32 LocalUserNum);

	// UOnlineBlueprintCallProxyBase接口
	virtual void Activate() override;
	// UOnlineBlueprintCallProxyBase接口结束

private:
	// 操作完成时的内部回调，调用公共的成功/失败回调
	void OnCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorVal);

private:
	// 关联用户的控制器编号
	int32 LocalUserNumber;

	// 在线子系统执行的委托
	FOnLoginCompleteDelegate Delegate;

	// 注册的OnDestroySessionComplete委托的句柄
	FDelegateHandle DelegateHandle;

	// 此调用发生的世界上下文对象
	UObject* WorldContextObject;
};
