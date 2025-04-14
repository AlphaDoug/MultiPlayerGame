//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"

#include "UObject/UObjectIterator.h"

#include "ChineseIdentityLibrary.generated.h"


//通用会话日志
DECLARE_LOG_CATEGORY_EXTERN(ChineseIdentityLog, Log, All);

UCLASS()
class UChineseIdentityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//********* 身份函数 *************//

	// 获取本地玩家的登录状态
	UFUNCTION(BlueprintCallable, Category = "中文会话|身份", meta = (ExpandEnumAsExecs = "Result", DisplayName = "获取登录状态"))
	static void GetLoginStatus(UPARAM(DisplayName = "唯一网络ID") const FBPUniqueNetId & UniqueNetID, UPARAM(DisplayName = "登录状态") EBPLoginStatus & LoginStatus, UPARAM(DisplayName = "结果") EBlueprintResultSwitch &Result);

	// 获取本地玩家的认证令牌
	UFUNCTION(BlueprintCallable, Category = "中文会话|身份", meta = (ExpandEnumAsExecs = "Result", DisplayName = "获取玩家身份验证令牌"))
	static void GetPlayerAuthToken(UPARAM(DisplayName = "玩家控制器") APlayerController * PlayerController, UPARAM(DisplayName = "认证令牌") FString & AuthToken, UPARAM(DisplayName = "结果") EBlueprintResultSwitch &Result);

	// 获取玩家的昵称
	UFUNCTION(BlueprintPure, Category = "中文会话|身份", meta = (DisplayName = "获取玩家的昵称"))
	static void GetPlayerNickname(UPARAM(DisplayName = "唯一网络ID") const FBPUniqueNetId & UniqueNetID, UPARAM(DisplayName = "玩家昵称") FString & PlayerNickname);

	//********* 用户账户函数 *************//

// 获取用户账户
	UFUNCTION(BlueprintCallable, Category = "中文会话|身份|用户账户", meta = (ExpandEnumAsExecs = "Result", DisplayName = "获取用户账户"))
		static void GetUserAccount(UPARAM(DisplayName = "唯一网络ID") const FBPUniqueNetId& UniqueNetId, UPARAM(DisplayName = "账户信息") FBPUserOnlineAccount& AccountInfo, UPARAM(DisplayName = "结果") EBlueprintResultSwitch& Result);

	// 获取所有已知用户账户
	UFUNCTION(BlueprintCallable, Category = "中文会话|身份|用户账户", meta = (ExpandEnumAsExecs = "Result", DisplayName = "获取所有已知用户账户"))
		static void GetAllUserAccounts(UPARAM(DisplayName = "账户信息") TArray<FBPUserOnlineAccount>& AccountInfos, UPARAM(DisplayName = "结果") EBlueprintResultSwitch& Result);

	// 获取用户账户访问令牌
	UFUNCTION(BlueprintPure, Category = "中文会话|身份|用户账户", meta = (DisplayName = "获取用户账户访问令牌"))
		static void GetUserAccountAccessToken(UPARAM(DisplayName = "账户信息") const FBPUserOnlineAccount& AccountInfo, UPARAM(DisplayName = "访问令牌") FString& AccessToken);

	// 获取用户账户Auth属性（取决于子系统）
	UFUNCTION(BlueprintCallable, Category = "中文会话|身份|用户账户", meta = (ExpandEnumAsExecs = "Result", DisplayName = "获取用户账户Auth属性"))
		static void GetUserAccountAuthAttribute(UPARAM(DisplayName = "账户信息") const FBPUserOnlineAccount& AccountInfo, const FString& AttributeName, UPARAM(DisplayName = "Auth属性") FString& AuthAttribute, UPARAM(DisplayName = "结果") EBlueprintResultSwitch& Result);

	// 设置用户账户属性（取决于子系统）
	UFUNCTION(BlueprintCallable, Category = "中文会话|身份|用户账户", meta = (ExpandEnumAsExecs = "Result", DisplayName = "设置用户账户属性"))
		static void SetUserAccountAttribute(const FBPUserOnlineAccount& AccountInfo, const FString& AttributeName, const FString& NewAttributeValue, EBlueprintResultSwitch& Result);

	// 获取用户ID
	UFUNCTION(BlueprintPure, Category = "中文会话|身份|用户账户", meta = (DisplayName = "获取用户ID"))
		static void GetUserID(UPARAM(DisplayName = "账户信息") const FBPUserOnlineAccount& AccountInfo, UPARAM(DisplayName = "唯一网络ID") FBPUniqueNetId& UniqueNetID);

	// 如果可能，获取用户账户的真实姓名
	UFUNCTION(BlueprintPure, Category = "中文会话|身份|用户账户", meta = (DisplayName = "获取用户账户的真实姓名"))
		static void GetUserAccountRealName(UPARAM(DisplayName = "账户信息") const FBPUserOnlineAccount& AccountInfo, UPARAM(DisplayName = "用户名") FString& UserName);

	// 如果可能，获取用户账户的显示名称
	UFUNCTION(BlueprintPure, Category = "中文会话|身份|用户账户", meta = (DisplayName = "获取用户账户的显示名称"))
		static void GetUserAccountDisplayName(UPARAM(DisplayName = "账户信息") const FBPUserOnlineAccount& AccountInfo, UPARAM(DisplayName = "显示名称") FString& DisplayName);

	// 获取用户账户属性（取决于子系统）
	UFUNCTION(BlueprintCallable, Category = "中文会话|身份|用户账户", meta = (ExpandEnumAsExecs = "Result", DisplayName = "获取用户账户属性"))
		static void GetUserAccountAttribute(UPARAM(DisplayName = "账户信息") const FBPUserOnlineAccount& AccountInfo, UPARAM(DisplayName = "属性名称") const FString& AttributeName, UPARAM(DisplayName = "属性值") FString& AttributeValue, UPARAM(DisplayName = "结果") EBlueprintResultSwitch& Result);


};	
