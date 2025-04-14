//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Online.h"
#include "Engine/LocalPlayer.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Interfaces/OnlineMessageInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "UObject/UObjectIterator.h"

#include "ChineseFriendsLibrary.generated.h"


//通用会话日志
DECLARE_LOG_CATEGORY_EXTERN(ChineseFriendsLog, Log, All);

UCLASS()
class UChineseFriendsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	//********* 好友列表功能 *************//

	// 向一群好友发送邀请，邀请他们加入当前的在线会话
	UFUNCTION(BlueprintCallable, Category = "中文会话|好友|好友列表", meta = (ExpandEnumAsExecs = "Result", DisplayName = "向好友们发送会话邀请"))
	static void SendSessionInviteToFriends(UPARAM(DisplayName = "玩家控制器") APlayerController *PlayerController, UPARAM(DisplayName = "好友们的唯一网络ID") const TArray<FBPUniqueNetId> &Friends, UPARAM(DisplayName = "结果") EBlueprintResultSwitch &Result);

	// 向一个朋友发送邀请，邀请他加入当前的在线会话
	UFUNCTION(BlueprintCallable, Category = "中文会话|好友|好友列表", meta = (ExpandEnumAsExecs = "Result", DisplayName = "向好友发送会话邀请"))
	static void SendSessionInviteToFriend(UPARAM(DisplayName = "玩家控制器") APlayerController *PlayerController, UPARAM(DisplayName = "好友唯一网络ID") const FBPUniqueNetId &FriendUniqueNetId, UPARAM(DisplayName = "结果") EBlueprintResultSwitch &Result);

	// 从之前读取/保存的好友列表中获取一个朋友（必须先调用“获取好友列表”，这样才能返回任何内容）
	UFUNCTION(BlueprintCallable, Category = "中文会话|好友|好友列表", meta = (DisplayName = "获取好友"))
	static void GetFriend(UPARAM(DisplayName = "玩家控制器") APlayerController *PlayerController, UPARAM(DisplayName = "好友唯一网络ID") const FBPUniqueNetId FriendUniqueNetId, UPARAM(DisplayName = "好友") FBPFriendInfo &Friend);

	// 获取之前读取/保存的好友列表（必须先调用“获取好友列表”，这样才能返回任何内容）
	UFUNCTION(BlueprintCallable, Category = "中文会话|好友|好友列表", meta = (DisplayName = "获取存储的朋友列表"))
	static void GetStoredFriendsList(UPARAM(DisplayName = "玩家控制器") APlayerController *PlayerController, UPARAM(DisplayName = "好友列表") TArray<FBPFriendInfo> &FriendsList);

	// 获取之前读取/保存的最近玩家列表（必须先调用“获取最近的玩家”，这样才能返回任何内容）
	UFUNCTION(BlueprintCallable, Category = "中文会话|好友|最近玩家列表", meta = (DisplayName = "获取存储的最近玩家列表"))
	static void GetStoredRecentPlayersList(UPARAM(DisplayName = "好友唯一网络ID") FBPUniqueNetId UniqueNetId, UPARAM(DisplayName = "玩家列表") TArray<FBPOnlineRecentPlayer> &PlayersList);

	//检查一个网络唯一ID是否是朋友
	UFUNCTION(BlueprintPure, Category = "中文会话|好友|好友列表", meta = (DisplayName = "是否为好友"))
	static void IsAFriend(UPARAM(DisplayName = "玩家控制器") APlayerController *PlayerController, UPARAM(DisplayName = "好友唯一网络ID") const FBPUniqueNetId UniqueNetId, UPARAM(DisplayName = "是否为好友") bool &IsFriend);
};	
