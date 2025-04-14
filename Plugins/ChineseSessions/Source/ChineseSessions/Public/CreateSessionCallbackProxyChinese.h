//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "BlueprintDataDefinitions.h"
#include "CreateSessionCallbackProxyChinese.generated.h"

UCLASS(MinimalAPI)
class UCreateSessionCallbackProxyChinese : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// Called when the session was created successfully
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FEmptyOnlineDelegate OnSuccess;

	// Called when there was an error creating the session
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FEmptyOnlineDelegate OnFailure;

	/**
	 *    使用默认的在线子系统创建一个会话，并提供高级可选输入。对于专用服务器，将UsePresence设置为false，并将IsDedicatedServer设置为true。专用服务器不使用presence。
	 *    @param PublicConnections  当进行'listen'服务器时，这个值必须>=2（ListenServer本身算作一个连接）
	 *    @param bUseLAN            当你想进行LAN游戏时，必须加载带有选项'bIsLanMatch'的关卡
	 *    @param bUsePresence       对于'listen'服务器（地图必须加载带有'listen'选项），这个值必须为true，对于'dedicated'服务器，这个值为false。
	 *    @param bUseLobbiesIfAvailable 用于标记子系统在API支持的情况下使用大厅API，而不是一般的托管，通常在steam上对于listen服务器为true，对于专用服务器为false
	 *    @param bShouldAdvertise   当OnlineSubsystem应该在某人搜索服务器时列出你的服务器时，设置为true。否则，服务器是隐藏的，只能通过邀请加入。
	 *    @param bUseLobbiesVoiceChatIfAvailable 如果API支持，设置为true以设置语音聊天大厅
	 *    @param bStartAfterCreate 创建会话后设置为true以开始会话。如果为false，你需要在准备好后手动调用StartSession。
	 */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject",AutoCreateRefTerm="ExtraSettings" ,DisplayName = "创建中文会话"), Category = "中文会话|在线|会话")
		static UCreateSessionCallbackProxyChinese* CreateChineseSession(UObject* WorldContextObject, UPARAM(DisplayName = "额外设置") const TArray<FSessionPropertyKeyPair>& ExtraSettings, UPARAM(DisplayName = "玩家控制器") class APlayerController* PlayerController = NULL, UPARAM(DisplayName = "公共连接") int32 PublicConnections = 100, UPARAM(DisplayName = "私人连接") int32 PrivateConnections = 0, UPARAM(DisplayName = "是否使用局域网") bool bUseLAN = false, UPARAM(DisplayName = "是否允许邀请") bool bAllowInvites = true, UPARAM(DisplayName = "是否为专用服务器") bool bIsDedicatedServer = false, UPARAM(DisplayName = "使用状态（listen服务器必须为“true”)") bool bUsePresence = true, UPARAM(DisplayName = "是否使用大厅（如果平台支持，例如Steam的listen服务器为“true”）") bool bUseLobbiesIfAvailable = true,UPARAM(DisplayName = "是否允许通过在线状态加入") bool bAllowJoinViaPresence = true, UPARAM(DisplayName = "是否仅允许通过好友的在线状态加入") bool bAllowJoinViaPresenceFriendsOnly = false, UPARAM(DisplayName = "是否防作弊保护") bool bAntiCheatProtected = false, UPARAM(DisplayName = "是否使用统计") bool bUsesStats = false, UPARAM(DisplayName = "是否广播") bool bShouldAdvertise = true, UPARAM(DisplayName = "是否使用大厅语音聊天（如果可用）") bool bUseLobbiesVoiceChatIfAvailable = false, UPARAM(DisplayName = "是否在创建后开始") bool bStartAfterCreate = true);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// 当会话创建完成的内部回调，可选地调用StartSession
	void OnCreateCompleted(FName SessionName, bool bWasSuccessful);

	// 当会话开始完成的内部回调
	void OnStartCompleted(FName SessionName, bool bWasSuccessful);

	// 触发事情的玩家控制器
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// 由在线子系统执行的委托
	FOnCreateSessionCompleteDelegate CreateCompleteDelegate;

	// 由在线子系统执行的委托
	FOnStartSessionCompleteDelegate StartCompleteDelegate;

	// 上述注册委托的句柄
	FDelegateHandle CreateCompleteDelegateHandle;
	FDelegateHandle StartCompleteDelegateHandle;

	// 公共连接的数量
	int NumPublicConnections;

	// 私有连接的数量
	int NumPrivateConnections;

	// 是否搜索局域网
	bool bUseLAN;

	// 是否允许邀请
	bool bAllowInvites;

	// 这是否是一个专用服务器
	bool bDedicatedServer;

	// 是否使用存在选项
	bool bUsePresence;

	// 如果api支持，是否优先使用大厅进行托管
	bool bUseLobbiesIfAvailable;

	// 是否允许通过存在加入
	bool bAllowJoinViaPresence;

	// 仅允许好友通过存在加入
	bool bAllowJoinViaPresenceFriendsOnly;

	// 声明服务器受到反作弊保护
	bool bAntiCheatProtected;

	// 记录统计数据
	bool bUsesStats;

	// 应该宣传服务器吗？
	bool bShouldAdvertise;

	// 如果api支持，是否优先使用语音聊天大厅
	bool bUseLobbiesVoiceChatIfAvailable;

	// 是否在创建后自动开始会话
	bool bStartAfterCreate;

	// 存储额外设置
	TArray<FSessionPropertyKeyPair> ExtraSettings;

	// 进行此调用的世界上下文对象
	UObject* WorldContextObject;
};

