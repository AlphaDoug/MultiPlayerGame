//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "BlueprintDataDefinitions.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Interfaces/OnlineMessageInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"


#include "ChineseSessionsLibrary.generated.h"


//通用会话日志
DECLARE_LOG_CATEGORY_EXTERN(ChineseSessionsLog, Log, All);


UCLASS()
class UChineseSessionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		//********* 会话管理功能 *************//

		// 从当前活动的游戏会话中踢出一个玩家，只在服务器上可用
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话", meta = (WorldContext = "WorldContextObject", DisplayName = "踢出玩家"))
		static bool KickPlayer(UPARAM(DisplayName = "世界上下文对象") UObject* WorldContextObject, UPARAM(DisplayName = "被踢玩家") APlayerController* PlayerToKick, UPARAM(DisplayName = "踢出原因") FText KickReason);

		// 禁止一个玩家参加当前活动的游戏会话，只在服务器上可用
		// 请注意，默认的游戏会话类并未实现实际的禁止名单，只是在调用此功能时踢出玩家
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话", meta = (WorldContext = "WorldContextObject", DisplayName = "封禁玩家"))
		static bool BanPlayer(UPARAM(DisplayName = "世界上下文对象") UObject* WorldContextObject, UPARAM(DisplayName = "被封玩家") APlayerController* PlayerToBan, UPARAM(DisplayName = "封禁原因") FText BanReason);

		//********* 会话搜索功能 *************//	

		// 根据会话设置是否已经存在，将其添加或修改到现有数组中
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (DisplayName = "添加或修改额外设置"))
		static void AddOrModifyExtraSettings(UPARAM(ref, DisplayName = "设置数组")  TArray<FSessionPropertyKeyPair> & SettingsArray, UPARAM(ref, DisplayName = "新的或更改的设置")  TArray<FSessionPropertyKeyPair> & NewOrChangedSettings, UPARAM(DisplayName = "修改后的设置数组") TArray<FSessionPropertyKeyPair> & ModifiedSettingsArray);

		// 从会话搜索结果中获取会话设置的数组
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (DisplayName = "获取额外设置"))
		static void GetExtraSettings(UPARAM(DisplayName = "会话结果") FBlueprintSessionResult SessionResult, UPARAM(DisplayName = "额外设置") TArray<FSessionPropertyKeyPair> & ExtraSettings);

		// 获取当前会话状态
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (WorldContext = "WorldContextObject"), DisplayName = "获取会话状态")
		static void GetSessionState(UObject* WorldContextObject, UPARAM(DisplayName = "会话状态") EBPOnlineSessionState &SessionState);

		// 获取当前会话设置
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "Result", WorldContext = "WorldContextObject", DisplayName = "获取中文会话设置"))
		static void GetSessionSettings(UObject* WorldContextObject, UPARAM(DisplayName = "连接数") int32 &NumConnections, UPARAM(DisplayName = "私有连接数") int32 &NumPrivateConnections, UPARAM(DisplayName = "是否为局域网") bool &bIsLAN, UPARAM(DisplayName = "是否为专用服务器") bool &bIsDedicated, UPARAM(DisplayName = "是否允许邀请") bool &bAllowInvites, UPARAM(DisplayName = "是否允许加入进行中的游戏") bool &bAllowJoinInProgress, UPARAM(DisplayName = "是否启用反作弊") bool &bIsAnticheatEnabled, UPARAM(DisplayName = "构建唯一ID") int32 &BuildUniqueID, UPARAM(DisplayName = "额外设置") TArray<FSessionPropertyKeyPair> &ExtraSettings, UPARAM(DisplayName = "结果") EBlueprintResultSwitch &Result);

		// 检查某人是否在当前会话中
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (WorldContext = "WorldContextObject", DisplayName = "判断玩家是否在会话中"))
		static void IsPlayerInSession(UObject* WorldContextObject, UPARAM(DisplayName = "待检查的玩家") const FBPUniqueNetId &PlayerToCheck, UPARAM(DisplayName = "是否在会话中") bool &bIsInSession);
		
		// 创建一个字面的会话搜索参数
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息|值", meta = (DisplayName = "创建字面会话搜索属性"))
		static FSessionsSearchSetting MakeLiteralSessionSearchProperty(UPARAM(DisplayName = "会话搜索属性") FSessionPropertyKeyPair SessionSearchProperty, UPARAM(DisplayName = "比较操作") EOnlineComparisonOpRedux ComparisonOp);


		//********* 会话信息函数 ***********//

		// 检查会话结果是否有效
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息", meta = (DisplayName = "检查会话结果是否有效"))
		static bool IsValidSession(UPARAM(DisplayName = "会话结果") const FBlueprintSessionResult & SessionResult);

		// 获取会话ID的字符串副本
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息", meta = (DisplayName = "获取会话ID的字符串副本"))
			static void GetSessionID_AsString(UPARAM(DisplayName = "会话结果") const FBlueprintSessionResult& SessionResult, UPARAM(DisplayName = "会话ID") FString& SessionID);

		// 获取当前会话ID的字符串副本
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息", meta = (WorldContext = "WorldContextObject", DisplayName = "获取当前会话ID的字符串副本"))
			static void GetCurrentSessionID_AsString(UObject* WorldContextObject, UPARAM(DisplayName = "会话ID") FString& SessionID);

		// 获取唯一的当前构建ID
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息", meta = (DisplayName = "获取唯一的当前构建ID"))
			static void GetCurrentUniqueBuildID(UPARAM(DisplayName = "唯一构建ID") int32& UniqueBuildId);

		// 从会话搜索结果中获取唯一的构建ID
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息", meta = (DisplayName = "从会话搜索结果中获取唯一的构建ID"))
			static void GetUniqueBuildID(UPARAM(DisplayName = "会话结果") FBlueprintSessionResult SessionResult, UPARAM(DisplayName = "唯一构建ID") int32& UniqueBuildId);


		// 获取会话属性键名值
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (DisplayName = "获取会话属性键名值"))
			static FName GetSessionPropertyKey(UPARAM(DisplayName = "会话属性") const FSessionPropertyKeyPair& SessionProperty);

		// 通过名称查找会话属性
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "Result", DisplayName = "通过名称查找会话属性"))
			static void FindSessionPropertyByName(UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "设置名称") FName SettingsName, UPARAM(DisplayName = "结果") EBlueprintResultSwitch& Result, UPARAM(DisplayName = "输出属性") FSessionPropertyKeyPair& OutProperty);

		// 通过名称查找会话属性索引
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "Result", DisplayName = "通过名称查找会话属性索引"))
			static void FindSessionPropertyIndexByName(UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "设置名称") FName SettingName, UPARAM(DisplayName = "结果") EBlueprintResultSwitch& Result, int32& OutIndex);

		// 获取会话自定义信息键/值为字节（用于枚举）
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "SearchResult", DisplayName = "获取中文会话额外设置字节"))
			static void GetSessionPropertyByte(UPARAM(DisplayName = "额外设置")const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "设置名称") FName SettingName, UPARAM(DisplayName = "搜索结果") ESessionSettingSearchResult& SearchResult, UPARAM(DisplayName = "设置值") uint8& SettingValue);

		// 获取会话自定义信息键/值为布尔值
		// Steam目前只支持Int，Float，String，BYTE值进行搜索过滤!!!
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "SearchResult", DisplayName = "获取中文会话额外设置布尔值"))
			static void GetSessionPropertyBool(UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "设置名称") FName SettingName, ESessionSettingSearchResult& SearchResult, UPARAM(DisplayName = "设置值") bool& SettingValue);

		// 获取会话自定义信息键/值为字符串
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "SearchResult", DisplayName = "获取中文会话额外设置字符串"))
			static void GetSessionPropertyString(UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "设置名称") FName SettingName, UPARAM(DisplayName = "搜索结果") ESessionSettingSearchResult& SearchResult, UPARAM(DisplayName = "设置值") FString& SettingValue);

		// 获取会话自定义信息键/值为整数
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "SearchResult", DisplayName = "获取中文会话额外设置整数"))
			static void GetSessionPropertyInt(UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "设置名称") FName SettingName, ESessionSettingSearchResult& SearchResult, UPARAM(DisplayName = "设置值") int32& SettingValue);

		// 获取会话自定义信息键/值为浮点数
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|会话信息", meta = (ExpandEnumAsExecs = "SearchResult", DisplayName = "获取中文会话额外设置浮点数"))
			static void GetSessionPropertyFloat(UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "设置名称") FName SettingName, UPARAM(DisplayName = "搜索结果") ESessionSettingSearchResult& SearchResult, UPARAM(DisplayName = "设置值") float& SettingValue);


		// 从字节（对于枚举）创建一个字面会话自定义信息键/值对
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息|值", meta = (DisplayName = "创建中文会话额外设置字节"))
			static FSessionPropertyKeyPair MakeLiteralSessionPropertyByte(UPARAM(DisplayName = "设置名称") FName Key, UPARAM(DisplayName = "设置值") uint8 Value);

		// 从布尔值创建一个字面会话自定义信息键/值对
		// Steam目前只支持Int，Float，String，BYTE值进行搜索过滤！
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息|值", meta = (DisplayName = "创建中文会话额外设置布尔值"))
			static FSessionPropertyKeyPair MakeLiteralSessionPropertyBool(UPARAM(DisplayName = "设置名称") FName Key, UPARAM(DisplayName = "设置值") bool Value);

		// 从字符串创建一个字面会话自定义信息键/值对
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息|值", meta = (DisplayName = "创建中文会话额外设置字符串"))
			static FSessionPropertyKeyPair MakeLiteralSessionPropertyString(UPARAM(DisplayName = "设置名称") FName Key, UPARAM(DisplayName = "设置值") FString Value);

		// 从整数创建一个字面会话自定义信息键/值对
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息|值", meta = (DisplayName = "创建中文会话额外设置整数"))
			static FSessionPropertyKeyPair MakeLiteralSessionPropertyInt(UPARAM(DisplayName = "设置名称") FName Key, UPARAM(DisplayName = "设置值") int32 Value);

		// 从浮点数创建一个字面会话自定义信息键/值对
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|会话信息|值", meta = (DisplayName = "创建中文会话额外设置点数"))
			static FSessionPropertyKeyPair MakeLiteralSessionPropertyFloat(UPARAM(DisplayName = "设置名称") FName Key, UPARAM(DisplayName = "设置值") float Value);


		//******* 玩家ID函数 *********//

		// 获取附加到给定控制器的网络玩家的唯一网络ID
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|玩家信息|玩家ID", meta = (DisplayName = "获取附加到给定控制器的网络玩家的唯一网络ID"))
		static void GetUniqueNetID(UPARAM(DisplayName = "玩家控制器") APlayerController *PlayerController, UPARAM(DisplayName = "唯一网络ID") FBPUniqueNetId &UniqueNetId);

		// 获取被分配给给定玩家状态的网络玩家的唯一网络ID。
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|玩家信息|玩家ID", meta = (DisplayName = "获取被分配给给定玩家状态的网络玩家的唯一网络ID"))
			static void GetUniqueNetIDFromPlayerState(UPARAM(DisplayName = "玩家状态") APlayerState* PlayerState, UPARAM(DisplayName = "唯一网络ID") FBPUniqueNetId& UniqueNetId);

		// 如果唯一网络ID有效，则返回True。
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|玩家信息|玩家ID", meta = (DisplayName = "如果唯一网络ID有效，则返回True"))
			static bool IsValidUniqueNetID(UPARAM(DisplayName = "唯一网络ID") const FBPUniqueNetId& UniqueNetId);

		/* 如果值相等则返回真（A == B）*/
		UFUNCTION(BlueprintPure, meta = (DisplayName = "唯一网络ID是否相等", CompactNodeTitle = "==", Keywords = "== equal"), Category = "中文会话|在线|会话|玩家信息|玩家ID")
			static bool EqualEqual_UNetIDUnetID(UPARAM(DisplayName = "A") const FBPUniqueNetId& A, UPARAM(DisplayName = "B") const FBPUniqueNetId& B);

		// 唯一网络ID转为字符串
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|唯一网络ID", meta = (DisplayName = "唯一网络ID转为字符串"))
			static void UniqueNetIdToString(UPARAM(DisplayName = "唯一网络ID") const FBPUniqueNetId& UniqueNetId, UPARAM(DisplayName = "字符串") FString& String);

		//******** 玩家名称功能 **********//

		// 获取玩家名字
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|玩家信息|玩家名字", meta = (DisplayName = "获取玩家名字"))
			static void GetPlayerName(UPARAM(DisplayName = "玩家控制器") APlayerController* PlayerController, UPARAM(DisplayName = "玩家名字") FString& PlayerName);

		// 设置附加到给定控制器的网络玩家的玩家名称
		UFUNCTION(BlueprintCallable, Category = "中文会话|在线|会话|玩家信息|玩家名字", meta = (DisplayName = "设置玩家名字"))
			static void SetPlayerName(UPARAM(DisplayName = "玩家控制器") APlayerController* PlayerController, UPARAM(DisplayName = "玩家名字") FString PlayerName);

		//********** 杂项玩家信息函数 *********//

		// 获取网络玩家的数量
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|玩家信息|杂项", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", DisplayName = "获取网络玩家数量"))
			static void GetNumberOfNetworkPlayers(UObject* WorldContextObject, UPARAM(DisplayName = "网络玩家数量") int32& NumNetPlayers);

		// 获取给定控制器的网络玩家索引
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|玩家信息|杂项", meta = (DisplayName = "获取给定控制器的网络玩家索引"))
			static void GetNetPlayerIndex(UPARAM(DisplayName = "玩家控制器") APlayerController* PlayerController, UPARAM(DisplayName = "网络玩家索引") int32& NetPlayerIndex);

		// 检查声明的会话子系统是否活动
		UFUNCTION(BlueprintPure, Category = "中文会话|在线|会话|杂项", meta = (DisplayName = "检查声明的会话子系统是否活动"))
			static bool HasOnlineSubsystem(UPARAM(DisplayName = "子系统名称") FName SubSystemName);

		//**** 无缝旅行函数 ****//

		//将服务器旅行暴露给蓝图
		UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "中文会话|在线|会话|无缝", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "服务器旅行"))
			static bool ServerTravel(UObject* WorldContextObject, UPARAM(DisplayName = "URL") const FString& InURL, UPARAM(DisplayName = "绝对") bool bAbsolute, UPARAM(DisplayName = "跳过游戏通知") bool bShouldSkipGameNotify);
		
};	
