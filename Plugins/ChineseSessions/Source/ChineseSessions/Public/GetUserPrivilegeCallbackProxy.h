//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "GetUserPrivilegeCallbackProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBlueprintGetUserPrivilegeDelegate,/* const &FBPUniqueNetId, PlayerID,*/ EBPUserPrivileges, QueriedPrivilege, bool, HadPrivilege);

UCLASS(MinimalAPI)
class UGetUserPrivilegeCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 当成功销毁时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FBlueprintGetUserPrivilegeDelegate OnSuccess;

	// 当销毁不成功时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FEmptyOnlineDelegate OnFailure;

	// 获取用户的权限
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject", DisplayName = "获取用户权限"), Category = "中文会话|在线|身份")
	static UGetUserPrivilegeCallbackProxy* GetUserPrivilege(UObject* WorldContextObject, UPARAM(DisplayName = "要检查的权限") const EBPUserPrivileges & PrivilegeToCheck, UPARAM(DisplayName = "玩家唯一网络ID") const FBPUniqueNetId & PlayerUniqueNetID);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when the operation completes, calls out to the public success/failure callbacks
	void OnCompleted(const FUniqueNetId& PlayerID, EUserPrivileges::Type Privilege, uint32 Result);

private:
	// 触发事物的玩家控制器
	FBPUniqueNetId PlayerUniqueNetID;

	// 要检查的权限
	EBPUserPrivileges UserPrivilege;

	// 这个调用正在发生的世界上下文对象
	UObject* WorldContextObject;
};
