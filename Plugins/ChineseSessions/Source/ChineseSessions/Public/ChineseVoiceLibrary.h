//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintDataDefinitions.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "Interfaces/VoiceInterface.h"
#include "Engine/GameInstance.h"

#include "UObject/UObjectIterator.h"

#include "ChineseVoiceLibrary.generated.h"


//通用会话日志
DECLARE_LOG_CATEGORY_EXTERN(ChineseVoiceLog, Log, All);


UCLASS()
class UChineseVoiceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	//********* 语音库函数 *************//

	// 获取指定本地用户是否有耳机
	UFUNCTION(BlueprintPure, Category = "中文会话|在线|语音|语音信息", meta = (DisplayName = "检查是否存在耳机"))
	static void IsHeadsetPresent(UPARAM(DisplayName = "是否存在耳机") bool & bHasHeadset, UPARAM(DisplayName = "本地玩家的编号")  uint8 LocalPlayerNum = 0);

	// 启动网络语音，允许在与“停止网络语音”协调的情况下进行按键说话
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "启动网络语音"))
	static void StartNetworkedVoice(UPARAM(DisplayName = "本地玩家的编号") uint8 LocalPlayerNum = 0);

	// 停止网络语音，允许在与“启动网络语音”协调的情况下进行按键说话
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "停止网络语音"))
	static void StopNetworkedVoice(UPARAM(DisplayName = "本地玩家的编号") uint8 LocalPlayerNum = 0);

	// 将本地玩家注册为对语音数据感兴趣的人
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "注册本地语音通信"))
	static bool RegisterLocalTalker(UPARAM(DisplayName = "本地玩家的编号") uint8 LocalPlayerNum = 0);

	// 将所有已签入的玩家注册为本地说话者
	// 这已经自动完成，只有在取消注册某人后才手动执行此操作
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "注册所有本地语音通信"))
	static void RegisterAllLocalTalkers();

	// 将本地玩家取消注册为本地说话者
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "取消注册本地通话器"))
	static void UnRegisterLocalTalker(UPARAM(DisplayName = "本地玩家的编号") uint8 LocalPlayerNum = 0);

	// 将所有已签入的玩家取消注册为本地说话者
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "取消注册所有本地通话器"))
	static void UnRegisterAllLocalTalkers();

	// 将远程玩家注册为说话者
	// 这已经自动完成，只有在取消注册某人后才手动执行此操作。
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "注册远程通话器"))
	static bool RegisterRemoteTalker(UPARAM(DisplayName = "网络唯一ID") const FBPUniqueNetId& UniqueNetId);

	// 将远程玩家取消注册为说话者
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "取消注册远程通话器"))
	static bool UnRegisterRemoteTalker(UPARAM(DisplayName = "网络唯一ID") const FBPUniqueNetId& UniqueNetId);

	// 将所有远程玩家取消注册为说话者
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "移除所有远程通话器"))
	static void RemoveAllRemoteTalkers();

	// 返回本地玩家是否正在说话
	UFUNCTION(BlueprintPure, Category = "中文会话|在线|语音|语音信息", meta = (DisplayName = "本地玩家是否在说话"))
	static bool IsLocalPlayerTalking(UPARAM(DisplayName = "本地玩家的编号") uint8 LocalPlayerNum);

	// 返回远程玩家是否正在说话
	UFUNCTION(BlueprintPure, Category = "中文会话|在线|语音|语音信息", meta = (DisplayName = "远程玩家是否在说话"))
	static bool IsRemotePlayerTalking(UPARAM(DisplayName = "网络唯一ID") const FBPUniqueNetId& UniqueNetId);

	// 返回指定本地玩家是否将某玩家静音
	UFUNCTION(BlueprintPure, Category = "中文会话|在线|语音|语音信息", meta = (DisplayName = "玩家是否被静音"))
	static bool IsPlayerMuted(UPARAM(DisplayName = "正在检查本地用户数量") uint8 LocalUserNumChecking, UPARAM(DisplayName = "网络唯一ID") const FBPUniqueNetId& UniqueNetId);

	// 如果是系统范围为真，则尝试为玩家全局静音，否则只为指定的本地玩家静音与网络唯一ID关联的玩家
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "静音远程说话者"))
	static bool MuteRemoteTalker(UPARAM(DisplayName = "本地用户数量") uint8 LocalUserNum, UPARAM(DisplayName = "网络唯一ID") const FBPUniqueNetId& UniqueNetId, UPARAM(DisplayName = "是否系统范围内") bool bIsSystemWide = false);

	// 如果是系统范围为真，则尝试为玩家全局取消静音，否则只为指定的本地玩家取消静音与网络唯一ID关联的玩家
	UFUNCTION(BlueprintCallable, Category = "中文会话|在线|语音", meta = (DisplayName = "取消静音远程说话"))
	static bool UnMuteRemoteTalker(UPARAM(DisplayName = "本地用户数量") uint8 LocalUserNum, UPARAM(DisplayName = "网络唯一ID") const FBPUniqueNetId& UniqueNetId, UPARAM(DisplayName = "是否系统范围内") bool bIsSystemWide = false);

	// 获取此系统的本地说话者数量
	UFUNCTION(BlueprintPure, Category = "中文会话|在线|语音|语音信息", meta = (DisplayName = "获取本地说话者数量"))
	static void GetNumLocalTalkers(UPARAM(DisplayName = "本地说话者数量") int32 & NumLocalTalkers);
};	
