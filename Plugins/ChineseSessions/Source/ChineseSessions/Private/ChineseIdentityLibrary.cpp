//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023


#include "ChineseIdentityLibrary.h"

//General Log
DEFINE_LOG_CATEGORY(ChineseIdentityLog);


void UChineseIdentityLibrary::GetPlayerAuthToken(APlayerController * PlayerController, FString & AuthToken, EBlueprintResultSwitch &Result)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取玩家身份验证令牌： 传递了一个错误的玩家控制器！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerController->Player);

	if (!Player)
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取玩家身份验证令牌： 获取本地玩家失败！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface();

	if (!IdentityInterface.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取玩家身份验证令牌： 获取身份接口失败！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	AuthToken = IdentityInterface->GetAuthToken(Player->GetControllerId());
	Result = EBlueprintResultSwitch::OnSuccess;
}

void UChineseIdentityLibrary::GetPlayerNickname(const FBPUniqueNetId & UniqueNetID, FString & PlayerNickname)
{
	if (!UniqueNetID.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取玩家昵称： 传递了一个错误的玩家唯一网络ID！"));
		return;
	}

	IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface();

	if (!IdentityInterface.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取玩家昵称： Failed to get identity interface!"));
		return;
	}
	PlayerNickname = IdentityInterface->GetPlayerNickname(*UniqueNetID.GetUniqueNetId());
}


void UChineseIdentityLibrary::GetLoginStatus(const FBPUniqueNetId & UniqueNetID, EBPLoginStatus & LoginStatus, EBlueprintResultSwitch &Result)
{
	if (!UniqueNetID.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取登录状态： 传递了一个错误的玩家唯一网络ID！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface();

	if (!IdentityInterface.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取登录状态： 获取身份接口失败！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	LoginStatus = (EBPLoginStatus)IdentityInterface->GetLoginStatus(*UniqueNetID.GetUniqueNetId());
	Result = EBlueprintResultSwitch::OnSuccess;
}


void UChineseIdentityLibrary::GetAllUserAccounts(TArray<FBPUserOnlineAccount> & AccountInfos, EBlueprintResultSwitch &Result)
{
	IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface();

	if (!IdentityInterface.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取所有用户账户失败，无法获取身份接口！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	TArray<TSharedPtr<FUserOnlineAccount>> accountInfos = IdentityInterface->GetAllUserAccounts();

	for (int i = 0; i < accountInfos.Num(); ++i)
	{
		AccountInfos.Add(FBPUserOnlineAccount(accountInfos[i]));
	}

	Result = EBlueprintResultSwitch::OnSuccess;
}

void UChineseIdentityLibrary::GetUserAccount(const FBPUniqueNetId & UniqueNetId, FBPUserOnlineAccount & AccountInfo, EBlueprintResultSwitch &Result)
{
	IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface();

	if(!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户： 传递了一个错误的唯一网络ID！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (!IdentityInterface.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户： Failed to get identity interface!"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	TSharedPtr<FUserOnlineAccount> accountInfo = IdentityInterface->GetUserAccount(*UniqueNetId.GetUniqueNetId());

	if (!accountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户： 获取身份接口失败！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	AccountInfo.UserAccountInfo = accountInfo;
	Result = EBlueprintResultSwitch::OnSuccess;
}

void UChineseIdentityLibrary::GetUserAccountAccessToken(const FBPUserOnlineAccount & AccountInfo, FString & AccessToken)
{
	if (!AccountInfo.UserAccountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户访问令牌 传递了一个无效的账户！"));
		return;
	}

	AccessToken = AccountInfo.UserAccountInfo->GetAccessToken();
}

void UChineseIdentityLibrary::GetUserAccountAuthAttribute(const FBPUserOnlineAccount & AccountInfo, const FString & AttributeName, FString & AuthAttribute, EBlueprintResultSwitch &Result)
{
	if (!AccountInfo.UserAccountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户身份验证属性： 传递了一个无效的账户！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (!AccountInfo.UserAccountInfo->GetAuthAttribute(AttributeName, AuthAttribute))
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户身份验证属性： 找不到该属性！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	Result = EBlueprintResultSwitch::OnSuccess;
}

void UChineseIdentityLibrary::SetUserAccountAttribute(const FBPUserOnlineAccount & AccountInfo, const FString & AttributeName, const FString & NewAttributeValue, EBlueprintResultSwitch &Result)
{
	if (!AccountInfo.UserAccountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("设置用户帐户身份验证属性： 传递了一个无效的账户！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (!AccountInfo.UserAccountInfo->SetUserAttribute(AttributeName, NewAttributeValue))
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("设置用户帐户身份验证属性： 无法设置该属性！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	Result = EBlueprintResultSwitch::OnSuccess;
}

void UChineseIdentityLibrary::GetUserID(const FBPUserOnlineAccount & AccountInfo, FBPUniqueNetId & UniqueNetID)
{
	if (!AccountInfo.UserAccountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户ID： 传递了一个无效的账户！"));
		return;
	}

	
	UniqueNetID.SetUniqueNetId(AccountInfo.UserAccountInfo->GetUserId());
}

void UChineseIdentityLibrary::GetUserAccountRealName(const FBPUserOnlineAccount & AccountInfo, FString & UserName)
{
	if (!AccountInfo.UserAccountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户真实名称： 传递了一个无效的账户！"));
		return;
	}


	UserName = AccountInfo.UserAccountInfo->GetRealName();
}

void UChineseIdentityLibrary::GetUserAccountDisplayName(const FBPUserOnlineAccount & AccountInfo, FString & DisplayName)
{
	if (!AccountInfo.UserAccountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户显示名称： 传递了一个无效的账户！"));
		return;
	}


	DisplayName = AccountInfo.UserAccountInfo->GetDisplayName();
}

void UChineseIdentityLibrary::GetUserAccountAttribute(const FBPUserOnlineAccount & AccountInfo, const FString & AttributeName, FString & AttributeValue, EBlueprintResultSwitch &Result)
{
	if (!AccountInfo.UserAccountInfo.IsValid())
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户属性： 传递了一个无效的账户！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (!AccountInfo.UserAccountInfo->GetUserAttribute(AttributeName, AttributeValue))
	{
		UE_LOG(ChineseIdentityLog, Warning, TEXT("获取用户帐户属性： 获取用户属性失败！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	Result = EBlueprintResultSwitch::OnSuccess;
}