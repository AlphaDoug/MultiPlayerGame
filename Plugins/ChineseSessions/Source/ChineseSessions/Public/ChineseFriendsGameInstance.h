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
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "UObject/UObjectIterator.h"
#include "ChineseFriendsInterface.h"

#include "ChineseFriendsGameInstance.generated.h"


//通用会话日志
	DECLARE_LOG_CATEGORY_EXTERN(ChineseFriendsInterfaceLog, Log, All);

UCLASS(meta = (DisplayName = "中文会话好友联机游戏实例"))
class CHINESESESSIONS_API UChineseFriendsGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UChineseFriendsGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|好友", meta = (DisplayName = "在玩家控制器上调用好友接口事件"))
		bool bCallFriendInterfaceEventsOnPlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|好友", meta = (DisplayName = "在玩家控制器上调用身份接口事件"))
	bool bCallIdentityInterfaceEventsOnPlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|好友", meta = (DisplayName = "在玩家控制器上调用语音接口事件"))
	bool bCallVoiceInterfaceEventsOnPlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|语音", meta = (DisplayName = "启用通话状态代理"))
	bool bEnableTalkingStatusDelegate;

	virtual void Shutdown() override;
	virtual void Init() override;

	//***本地收到会话邀请***//
	FOnSessionInviteReceivedDelegate SessionInviteReceivedDelegate;
	FDelegateHandle SessionInviteReceivedDelegateHandle;

	//const FUniqueNetId& /用户ID/, const FUniqueNetId& /来自ID/, const FString& /应用ID/, const FOnlineSessionSearchResult& /邀请结果/
	void OnSessionInviteReceivedMaster(const FUniqueNetId & PersonInvited, const FUniqueNetId & PersonInviting, const FString & AppId, const FOnlineSessionSearchResult& SessionToJoin);

	//本地玩家接受会话邀请后，触发此事件，根据会话结果调用“加入会话”加入
	UFUNCTION(BlueprintImplementableEvent, Category = "中文会话|好友", meta = (DisplayName = "收到会话邀请时"))
	void OnSessionInviteReceived(UPARAM(DisplayName = "本地玩家编号") int32 LocalPlayerNum, UPARAM(DisplayName = "邀请人的唯一网络ID") FBPUniqueNetId PersonInviting, UPARAM(DisplayName = "应用程序ID") const FString& AppId, UPARAM(DisplayName = "要加入的会话") const FBlueprintSessionResult& SessionToJoin);
	//应用程序ID、要加入的会话结果

	//*** 本地接受的会话邀请 ***//
	FOnSessionUserInviteAcceptedDelegate SessionInviteAcceptedDelegate;
	FDelegateHandle SessionInviteAcceptedDelegateHandle;

	void OnSessionInviteAcceptedMaster(const bool bWasSuccessful, int32 LocalPlayer, TSharedPtr<const FUniqueNetId> PersonInviting, const FOnlineSessionSearchResult& SessionToJoin);

	// 在本地玩家接受会话邀请后，将触发此事件，调用会话结果上的“加入会话”以加入会话
	// 这个函数目前并未在Epic的任何默认子系统中连接，它存在于此是为了自定义子系统
	//LocalPlayerNum:本地玩家编号
	//PersonInvited:被邀请人
	//SessionToJoin:要加入的会话
	UFUNCTION(BlueprintImplementableEvent, Category = "中文会话|好友", meta = (DisplayName = "会话邀请被接受时"))
	void OnSessionInviteAccepted(UPARAM(DisplayName = "本地玩家编号") int32 LocalPlayerNum, UPARAM(DisplayName = "被邀请人") FBPUniqueNetId PersonInvited, UPARAM(DisplayName = "要加入的会话") const FBlueprintSessionResult& SessionToJoin);


	// 如果"启用通话状态代理"属性为真，语音状态改变后将触发此事件
	UFUNCTION(BlueprintImplementableEvent, Category = "中文会话|语音", meta = (DisplayName = "对话状态改变时"))
	void OnPlayerTalkingStateChanged(FBPUniqueNetId PlayerId, bool bIsTalking);

	void OnPlayerTalkingStateChangedMaster(TSharedRef<const FUniqueNetId> PlayerId, bool bIsTalking);

	FOnPlayerTalkingStateChangedDelegate PlayerTalkingStateChangedDelegate;
	FDelegateHandle PlayerTalkingStateChangedDelegateHandle;


	// 当指定的LocalUser更改登录状态时调用
	UFUNCTION(BlueprintImplementableEvent , Category = "中文会话|身份", meta = (DisplayName = "玩家登录更改"))
	void OnPlayerLoginChanged(int32 PlayerNum);

	void OnPlayerLoginChangedMaster(int32 PlayerNum);
	FOnLoginChangedDelegate PlayerLoginChangedDelegate;
	FDelegateHandle PlayerLoginChangedDelegateHandle;

	// 当指定的LocalUser更改登录状态时被调用
	UFUNCTION(BlueprintImplementableEvent, Category = "中文会话|身份", meta = (DisplayName = "玩家登录状态改变"))
	void OnPlayerLoginStatusChanged(int32 PlayerNum, EBPLoginStatus PreviousStatus, EBPLoginStatus NewStatus, FBPUniqueNetId NewPlayerUniqueNetID);

	void OnPlayerLoginStatusChangedMaster(int32 PlayerNum, ELoginStatus::Type PreviousStatus, ELoginStatus::Type NewStatus, const FUniqueNetId & NewPlayerUniqueNetID);
	FOnLoginStatusChangedDelegate PlayerLoginStatusChangedDelegate;
	FDelegateHandle PlayerLoginStatusChangedDelegateHandle;

};

