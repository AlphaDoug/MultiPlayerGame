//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Interfaces/OnlineMessageInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "UObject/UObjectIterator.h"
#include "BlueprintDataDefinitions.h"
#include "ChineseFriendsInterface.generated.h"


UINTERFACE(MinimalAPI)
class UChineseFriendsInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IChineseFriendsInterface
{
	GENERATED_IINTERFACE_BODY()
public:
	
	// 当指定的本地用户接受了会话邀请时调用，使用结果上的“加入会话”进行连接
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "收到会话邀请时"))
	void OnSessionInviteReceived(FBPUniqueNetId PersonInviting, const FBlueprintSessionResult& SearchResult);

	// 当指定的本地用户接受了会话邀请时调用，使用结果上的“加入会话”进行连接
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "已接受会话邀请"))
	void OnSessionInviteAccepted(FBPUniqueNetId PersonInvited, const FBlueprintSessionResult& SearchResult);

	// 当指定的本地用户接受了会话邀请时调用，使用结果上的“加入会话”进行连接
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "玩家语音状态发生变化"))
	void OnPlayerVoiceStateChanged(FBPUniqueNetId PlayerId, bool bIsTalking);

	// 当指定的本地用户更改登录状态时调用
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "玩家登录更改"))
	void OnPlayerLoginChanged(int32 PlayerNum);

	// 当指定的本地用户更改登录状态时调用
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "玩家登录状态发生变化"))
	void OnPlayerLoginStatusChanged(EBPLoginStatus PreviousStatus, EBPLoginStatus NewStatus, FBPUniqueNetId PlayerUniqueNetID);

};	
