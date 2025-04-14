//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#include "GetUserPrivilegeCallbackProxy.h"


//////////////////////////////////////////////////////////////////////////
// UGetUserPrivilegeCallbackProxy

UGetUserPrivilegeCallbackProxy::UGetUserPrivilegeCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGetUserPrivilegeCallbackProxy* UGetUserPrivilegeCallbackProxy::GetUserPrivilege(UObject* WorldContextObject, const EBPUserPrivileges & PrivilegeToCheck, const FBPUniqueNetId & PlayerUniqueNetID)
{
	UGetUserPrivilegeCallbackProxy* Proxy = NewObject<UGetUserPrivilegeCallbackProxy>();
	Proxy->PlayerUniqueNetID.SetUniqueNetId(PlayerUniqueNetID.GetUniqueNetId());
	Proxy->UserPrivilege = PrivilegeToCheck;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UGetUserPrivilegeCallbackProxy::Activate()
{
	auto Identity = Online::GetIdentityInterface(GetWorld());

	if (Identity.IsValid())
	{
		Identity->GetUserPrivilege(*PlayerUniqueNetID.GetUniqueNetId(), (EUserPrivileges::Type)UserPrivilege, IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted));
		return;
	}

	// 立即失败
	OnFailure.Broadcast();
}

void UGetUserPrivilegeCallbackProxy::OnCompleted(const FUniqueNetId& PlayerID, EUserPrivileges::Type Privilege, uint32 PrivilegeResult)
{
	OnSuccess.Broadcast(/*PlayerID,*/ (EBPUserPrivileges)Privilege, PrivilegeResult == 0);
}
