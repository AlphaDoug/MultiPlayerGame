//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h"
#include "BlueprintDataDefinitions.h"
#include "FindSessionsCallbackProxyChinese.generated.h"


FORCEINLINE bool operator==(const FBlueprintSessionResult& A, const FBlueprintSessionResult& B)
{
	return (A.OnlineResult.IsValid() == B.OnlineResult.IsValid() && (A.OnlineResult.GetSessionIdStr() == B.OnlineResult.GetSessionIdStr()));
}

UCLASS(MinimalAPI)
class UFindSessionsCallbackProxyChinese : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// 查询成功时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "成功时"))
	FBlueprintFindSessionsResultDelegate OnSuccess;

	// 当查询失败时调用
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "失败时"))
	FBlueprintFindSessionsResultDelegate OnFailure;

	// 使用默认的在线子系统搜索公告的会话，并包括一组过滤器
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm="Filters"), Category = "中文会话|在线|会话", DisplayName = "查找中文会话")
	static UFindSessionsCallbackProxyChinese* FindSessionsChinese(UObject* WorldContextObject, UPARAM(DisplayName = "玩家控制器") class APlayerController* PlayerController, UPARAM(DisplayName = "最大结果数") int32 MaxResults, UPARAM(DisplayName = "是否使用局域网") bool bUseLAN, UPARAM(DisplayName = "要搜索的服务器类型") EBPServerPresenceSearchType ServerTypeToSearch, UPARAM(DisplayName = "过滤器") const TArray<FSessionsSearchSetting> &Filters, UPARAM(DisplayName = "是否仅空服务器") bool bEmptyServersOnly = false, UPARAM(DisplayName = "是否仅非空服务器") bool bNonEmptyServersOnly = false, UPARAM(DisplayName = "是否仅安全服务器") bool bSecureServersOnly = false, UPARAM(DisplayName = "是否搜索大厅") bool bSearchLobbies = true, UPARAM(DisplayName = "至少还有几个位置") int MinSlotsAvailable = 0);

	static bool CompareVariants(const FVariantData &A, const FVariantData &B, EOnlineComparisonOpRedux Comparator);
	
	// 根据给定的搜索参数过滤会话结果数组，返回过滤结果的新数组
	UFUNCTION(BluePrintCallable, meta = (Category = "中文会话|会话", DisplayName = "过滤会话结果"))
	static void FilterSessionResults(UPARAM(DisplayName = "会话结果") const TArray<FBlueprintSessionResult> &SessionResults, UPARAM(DisplayName = "过滤器") const TArray<FSessionsSearchSetting> &Filters, UPARAM(DisplayName = "过滤结果") TArray<FBlueprintSessionResult> &FilteredResults);
	

	// UOnlineBlueprintCallProxyBase接口
	virtual void Activate() override;
	// UOnlineBlueprintCallProxyBase接口结束

private:
	// 当会话搜索完成时的内部回调，调用公共的成功/失败回调
	void OnCompleted(bool bSuccess);

	bool bRunSecondSearch;
	bool bIsOnSecondSearch;

	TArray<FBlueprintSessionResult> SessionSearchResults;

private:
	// 触发事情的玩家控制器
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// 在线子系统执行的委托
	FOnFindSessionsCompleteDelegate Delegate;

	// 已注册的OnFindSessionsComplete委托的句柄
	FDelegateHandle DelegateHandle;

	// 用于跟踪搜索结果的对象
	TSharedPtr<FOnlineSessionSearch> SearchObject;
	TSharedPtr<FOnlineSessionSearch> SearchObjectDedicated;

	// 是否搜索局域网
	bool bUseLAN;

	// 是否搜索专用服务器
	EBPServerPresenceSearchType ServerSearchType;

	// 返回的最大结果数
	int MaxResults;

	// 存储额外设置
	TArray<FSessionsSearchSetting> SearchSettings;

	// 仅搜索空服务器
	bool bEmptyServersOnly;

	// 仅搜索非空服务器
	bool bNonEmptyServersOnly;

	// 仅搜索安全服务器
	bool bSecureServersOnly;

	// 搜索大厅
	bool bSearchLobbies;

	// 搜索所需的最小插槽数
	int MinSlotsAvailable;

	// 进行此调用的世界上下文对象
	UObject* WorldContextObject;
};
