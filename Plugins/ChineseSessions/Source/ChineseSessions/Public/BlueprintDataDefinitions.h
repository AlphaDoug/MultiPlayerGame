//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerState.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineDelegateMacros.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemImpl.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemUtilsModule.h"
#include "GameFramework/PlayerController.h"
#include "Modules/ModuleManager.h"
#include "OnlineSubsystemUtilsClasses.h"
#include "BlueprintDataDefinitions.generated.h"	

UENUM(BlueprintType, meta = (DisplayName = "用户权限"))
enum class EBPUserPrivileges : uint8
{
	/** 用户是否可以玩游戏，无论是在线还是离线 - 可能受年龄限制 */
	CanPlay UMETA(DisplayName = "可以玩游戏"),
	/** 用户是否可以在在线模式下玩游戏 */
	CanPlayOnline UMETA(DisplayName = "可以在线玩游戏"),
	/** 用户是否可以使用语音和文字聊天 */
	CanCommunicateOnline UMETA(DisplayName = "可以在线交流"),
	/** 用户是否可以使用其他用户生成的内容 */
	CanUseUserGeneratedContent UMETA(DisplayName = "可以使用用户生成的内容")
};


UENUM(BlueprintType, meta = (DisplayName = "登录状态"))
enum class EBPLoginStatus : uint8
{
	/** 玩家尚未登录或选择本地配置文件 */
	NotLoggedIn UMETA(DisplayName = "未登录"),
	/** 玩家正在使用本地配置文件，但未登录 */
	UsingLocalProfile UMETA(DisplayName = "使用本地配置文件"),
	/** 玩家已经通过平台特定的认证服务进行验证 */
	LoggedIn UMETA(DisplayName = "已登录")
};


USTRUCT(BlueprintType, meta = (DisplayName = "在线用户账户"))
struct FBPUserOnlineAccount
{
	GENERATED_USTRUCT_BODY()

public:
	TSharedPtr<FUserOnlineAccount> UserAccountInfo;

	FBPUserOnlineAccount()
	{

	}

	FBPUserOnlineAccount(TSharedPtr<FUserOnlineAccount> UserAccount)
	{
		UserAccountInfo = UserAccount;
	}
};

UENUM(BlueprintType, meta = (DisplayName = "会话设置搜索结果"))
enum class ESessionSettingSearchResult : uint8
{
	// 找到了设置
	Found UMETA(DisplayName = "找到了设置"),

	// 没有找到设置
	NotFound UMETA(DisplayName = "没有找到设置"),

	// 类型不正确
	WrongType UMETA(DisplayName = "类型不正确")
};

// 这使得很多蓝图函数更加清晰
UENUM(BlueprintType, meta = (DisplayName = "蓝图结果切换"))
enum class EBlueprintResultSwitch : uint8
{
	// 成功时
	OnSuccess UMETA(DisplayName = "成功时"),

	// 失败时
	OnFailure UMETA(DisplayName = "失败时")
};

// 这使得很多蓝图函数更加清晰
UENUM(BlueprintType, meta = (DisplayName = "蓝图异步结果切换"))
enum class EBlueprintAsyncResultSwitch : uint8
{
	// 成功时
	OnSuccess UMETA(DisplayName = "成功时"),

	// Still loading
	AsyncLoading UMETA(DisplayName = "加载中"),
	// 失败时
	OnFailure UMETA(DisplayName = "失败时")
};

// 这是用来定义服务器类型搜索的
UENUM(BlueprintType, meta = (DisplayName = "服务器存在搜索类型"))
enum class EBPServerPresenceSearchType : uint8
{
	AllServers UMETA(DisplayName = "所有服务器"),
	ClientServersOnly UMETA(DisplayName = "仅客户端服务器"),
	DedicatedServersOnly UMETA(DisplayName = "仅专用服务器")
};

// 希望这个能在编辑器中切换
UENUM(BlueprintType, meta = (DisplayName = "在线状态"))
enum class EBPOnlinePresenceState : uint8
{
	Online UMETA(DisplayName = "在线"),
	Offline UMETA(DisplayName = "离线"),
	Away UMETA(DisplayName = "离开"),
	ExtendedAway UMETA(DisplayName = "长时间离开"),
	DoNotDisturb UMETA(DisplayName = "请勿打扰"),
	Chat UMETA(DisplayName = "聊天")
};

UENUM(BlueprintType, meta = (DisplayName = "在线会话状态"))
enum class EBPOnlineSessionState : uint8
{
	/** 还未创建在线会话 */
	NoSession UMETA(DisplayName = "未创建会话"),
	/** 在线会话正在创建中 */
	Creating UMETA(DisplayName = "创建中"),
	/** 会话已创建但尚未开始（预比赛大厅） */
	Pending UMETA(DisplayName = "待开始"),
	/** 已请求开始会话（可能由于与后端通信需要时间） */
	Starting UMETA(DisplayName = "开始中"),
	/** 当前会话已开始。禁用进程中加入的会话不再可加入 */
	InProgress UMETA(DisplayName = "进行中"),
	/** 会话仍有效，但不再进行会话（比赛后大厅） */
	Ending UMETA(DisplayName = "结束中"),
	/** 会话已关闭并提交任何统计数据 */
	Ended UMETA(DisplayName = "已结束"),
	/** 会话正在被销毁 */
	Destroying UMETA(DisplayName = "销毁中")
};

// 现在木有啥思路
// UniqueNetId是一个抽象类，所以我不能重新初始化它来在一些函数上制作一个共享指针
// 所以我把可蓝图化的UniqueNetID变成了一个双变量结构体，并且我正在转换指针的const变量
// 后面考虑一下怎么优化

USTRUCT(BlueprintType, meta = (DisplayName = "唯一网络ID"))
struct FBPUniqueNetId
{
	GENERATED_USTRUCT_BODY()

private:
	bool bUseDirectPointer;


public:
	TSharedPtr<const FUniqueNetId> UniqueNetId;
	const FUniqueNetId * UniqueNetIdPtr;

	void SetUniqueNetId(const TSharedPtr<const FUniqueNetId> &ID)
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
		UniqueNetId = ID;
	}

	void SetUniqueNetId(const FUniqueNetId *ID)
	{
		bUseDirectPointer = true;
		UniqueNetIdPtr = ID;
	}

	bool IsValid() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr && UniqueNetIdPtr->IsValid())
		{
			return true;
		}
		else if (UniqueNetId.IsValid())
		{
			return true;
		}
		else
			return false;

	}

	const FUniqueNetId* GetUniqueNetId() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr)
		{
			// 不再转换为非const，因为现在所有函数都传递const UniqueNetIds
			return /*const_cast<FUniqueNetId*>*/(UniqueNetIdPtr);
		}
		else if (UniqueNetId.IsValid())
		{
			return UniqueNetId.Get();
		}
		else
			return nullptr;
	}

	// 添加一个比较运算符，以便std函数可以与此结构体一起工作
	FORCEINLINE bool operator==(const FBPUniqueNetId& Other) const
	{
		return (IsValid() && Other.IsValid() && (*GetUniqueNetId() == *Other.GetUniqueNetId()));
	}

	FORCEINLINE bool operator!=(const FBPUniqueNetId& Other) const
	{
		return !(IsValid() && Other.IsValid() && (*GetUniqueNetId() == *Other.GetUniqueNetId()));
	}

	FBPUniqueNetId()
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
	}
};

USTRUCT(BluePrintType, meta = (DisplayName = "在线用户"))
struct FBPOnlineUser
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "唯一网络ID"))
		FBPUniqueNetId UniqueNetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "显示名称"))
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "真实姓名"))
		FString RealName;
};

USTRUCT(BluePrintType, meta = (DisplayName = "在线近期玩家"))
struct FBPOnlineRecentPlayer : public FBPOnlineUser
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "最后在线时间"))
		FString LastSeen;
};


USTRUCT(BlueprintType, meta = (DisplayName = "好友在线信息"))
struct FBPFriendPresenceInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "是否在线"))
		bool bIsOnline = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "是否在玩游戏"))
		bool bIsPlaying = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "是否在玩这个游戏"))
		bool bIsPlayingThisGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "是否可加入"))
		bool bIsJoinable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "是否有语音支持"))
		bool bHasVoiceSupport = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "在线状态"))
		EBPOnlinePresenceState PresenceState = EBPOnlinePresenceState::Offline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "状态字符串"))
		FString StatusString;

	FBPFriendPresenceInfo()
	{
		bIsOnline = false;
		bIsPlaying = false;
		bIsPlayingThisGame = false;
		bIsJoinable = false;
		bHasVoiceSupport = false;
		PresenceState = EBPOnlinePresenceState::Offline;
	}
};

USTRUCT(BlueprintType, meta = (DisplayName = "好友信息"))
struct FBPFriendInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "显示名称"))
	FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "真实姓名"))
	FString RealName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "在线状态"))
	EBPOnlinePresenceState OnlineState = EBPOnlinePresenceState::Offline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "唯一网络ID"))
	FBPUniqueNetId UniqueNetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "是否在玩同一款游戏"))
	bool bIsPlayingSameGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "中文会话|在线|好友", meta = (DisplayName = "好友在线信息"))
	FBPFriendPresenceInfo PresenceInfo;

	FBPFriendInfo()
	{
		OnlineState = EBPOnlinePresenceState::Offline;
		bIsPlayingSameGame = false;
	}
};


/** 给定搜索查询的比较操作类型 */
// 用于比较会话属性
UENUM(BlueprintType, meta = (DisplayName = "在线比较操作Redux"))
enum class EOnlineComparisonOpRedux : uint8
{
	Equals UMETA(DisplayName = "等于"),
	NotEquals UMETA(DisplayName = "不等于"),
	GreaterThan UMETA(DisplayName = "大于"),
	GreaterThanEquals UMETA(DisplayName = "大于等于"),
	LessThan UMETA(DisplayName = "小于"),
	LessThanEquals UMETA(DisplayName = "小于等于"),
};


// 用于在转换为FVariantData之前存储会话属性
USTRUCT(BlueprintType, meta = (DisplayName = "会话属性键值对"))
struct FSessionPropertyKeyPair
{
	GENERATED_USTRUCT_BODY()

	FName Key;
	FVariantData Data;
};


// 发送给“寻找中文会话”以过滤最终结果
USTRUCT(BlueprintType, meta = (DisplayName = "会话搜索设置"))
struct FSessionsSearchSetting
{
	GENERATED_USTRUCT_BODY()
	


	// 由于原始版本未暴露给蓝图，因此必须复制一份
	/** 这个会话设置在后端搜索中是如何比较的 */
	EOnlineComparisonOpRedux ComparisonOp;

	// 要搜索的键值对
	FSessionPropertyKeyPair PropertyKeyPair;
};

// 由于默认的键值对未暴露给其他模块，因此必须在这里重新创建它
// 辅助类，用于各种方法以减少调用层次结构
struct FOnlineSubsystemBPCallHelperChinese
{
public:
	FOnlineSubsystemBPCallHelperChinese(const TCHAR* CallFunctionContext, UWorld* World, FName SystemName = NAME_None)
		: OnlineSub(Online::GetSubsystem(World, SystemName))
		, FunctionContext(CallFunctionContext)
	{
		if (OnlineSub == nullptr)
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - 无效或未初始化的OnlineSubsystem"), FunctionContext), ELogVerbosity::Warning);
		}
	}

	void QueryIDFromPlayerController(APlayerController* PlayerController)
	{
		UserID.Reset();
		
		if (APlayerState* PlayerState = (PlayerController != NULL) ? PlayerController->PlayerState : NULL)
		{
			UserID = PlayerState->GetUniqueId().GetUniqueNetId();
			if (!UserID.IsValid())
			{
				FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - 无法将本地玩家映射到唯一的网络ID"), FunctionContext), ELogVerbosity::Warning);
			}
		}
		else
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - 无效的玩家状态"), FunctionContext), ELogVerbosity::Warning);
		}
	}


	bool IsValid() const
	{
		return UserID.IsValid() && (OnlineSub != nullptr);
	}

public:
	
	TSharedPtr</*class*/ const FUniqueNetId> UserID;
	IOnlineSubsystem* const OnlineSub;
	const TCHAR* FunctionContext;
};
class FOnlineSearchSettingsEx : public FOnlineSearchSettings
{
	/**
	*	设置一个键值对组合，定义一个搜索参数
	*
	* @param Key 是设置的键
	* @param Value 是设置的值
	* @param InType 是比较的类型
	*/
public:

	void HardSet(FName Key, const FVariantData& Value, EOnlineComparisonOpRedux CompOp)
	{
		FOnlineSessionSearchParam* SearchParam = SearchParams.Find(Key);

		TEnumAsByte<EOnlineComparisonOp::Type> op;

		switch (CompOp)
		{
		case EOnlineComparisonOpRedux::Equals: op = EOnlineComparisonOp::Equals; break;
		case EOnlineComparisonOpRedux::GreaterThan: op = EOnlineComparisonOp::GreaterThan; break;
		case EOnlineComparisonOpRedux::GreaterThanEquals: op = EOnlineComparisonOp::GreaterThanEquals; break;
		case EOnlineComparisonOpRedux::LessThan: op = EOnlineComparisonOp::LessThan; break;
		case EOnlineComparisonOpRedux::LessThanEquals: op = EOnlineComparisonOp::LessThanEquals; break;
		case EOnlineComparisonOpRedux::NotEquals: op = EOnlineComparisonOp::NotEquals; break;
		default: op = EOnlineComparisonOp::Equals; break;
		}

		if (SearchParam)
		{
			SearchParam->Data = Value;
			SearchParam->ComparisonOp = op;
		}
		else
		{
			FOnlineSessionSearchParam searchSetting((int)0, op);
			searchSetting.Data = Value;
			SearchParams.Add(Key, searchSetting);
		}
	}
};

#define INVALID_INDEX -1