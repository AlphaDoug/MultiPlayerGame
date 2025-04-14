//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023


#include "ChineseSessionsLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"

//General Log
DEFINE_LOG_CATEGORY(ChineseSessionsLog);


bool UChineseSessionsLibrary::KickPlayer(UObject* WorldContextObject, APlayerController* PlayerToKick, FText KickReason)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (World)
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			if (GameMode->GameSession)
			{
				return GameMode->GameSession->KickPlayer(PlayerToKick, KickReason);
			}
		}
	}

	return false;
}

bool UChineseSessionsLibrary::BanPlayer(UObject* WorldContextObject, APlayerController* PlayerToBan, FText BanReason)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (World)
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			if (GameMode->GameSession)
			{
				return GameMode->GameSession->BanPlayer(PlayerToBan, BanReason);
			}
		}
	}

	return false;
}

bool UChineseSessionsLibrary::IsValidSession(const FBlueprintSessionResult & SessionResult)
{
	return SessionResult.OnlineResult.IsValid();
}

void UChineseSessionsLibrary::GetSessionID_AsString(const FBlueprintSessionResult & SessionResult, FString& SessionID)
{
	const TSharedPtr<class FOnlineSessionInfo> SessionInfo = SessionResult.OnlineResult.Session.SessionInfo;
	if (SessionInfo.IsValid() && SessionInfo->IsValid() && SessionInfo->GetSessionId().IsValid())
	{
		SessionID = SessionInfo->GetSessionId().ToString();
		return;
	}

	// Zero the string out if we didn't have a valid one, in case this is called in c++
	SessionID.Empty();
}

void UChineseSessionsLibrary::GetCurrentSessionID_AsString(UObject* WorldContextObject, FString& SessionID)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);

	if (!SessionInterface.IsValid()) 
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("获取当前会话ID 字符串： 无法获取会话接口！"));
		SessionID.Empty();
		return;
	}

	const FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
	if (Session != nullptr) 
	{
		const TSharedPtr<class FOnlineSessionInfo> SessionInfo = Session->SessionInfo;
		if (SessionInfo.IsValid() && SessionInfo->IsValid() && SessionInfo->GetSessionId().IsValid()) 
		{
			SessionID = SessionInfo->GetSessionId().ToString();
			return;
		}
	}

	// Zero the string out if we didn't have a valid one, in case this is called in c++
	SessionID.Empty();
}

void UChineseSessionsLibrary::GetCurrentUniqueBuildID(int32 &UniqueBuildId)
{
	UniqueBuildId = GetBuildUniqueId();
}

void UChineseSessionsLibrary::GetUniqueBuildID(FBlueprintSessionResult SessionResult, int32 &UniqueBuildId)
{
	UniqueBuildId = SessionResult.OnlineResult.Session.SessionSettings.BuildUniqueId;
}

FName UChineseSessionsLibrary::GetSessionPropertyKey(const FSessionPropertyKeyPair& SessionProperty)
{
	return SessionProperty.Key;
}

void UChineseSessionsLibrary::FindSessionPropertyByName(const TArray<FSessionPropertyKeyPair>& ExtraSettings, FName SettingName, EBlueprintResultSwitch &Result, FSessionPropertyKeyPair& OutProperty)
{
	const FSessionPropertyKeyPair* prop = ExtraSettings.FindByPredicate([&](const FSessionPropertyKeyPair& it) {return it.Key == SettingName; });
	if (prop)
	{
		Result = EBlueprintResultSwitch::OnSuccess;
		OutProperty = *prop;
		return;
	}

	Result = EBlueprintResultSwitch::OnFailure;
}

void UChineseSessionsLibrary::FindSessionPropertyIndexByName(const TArray<FSessionPropertyKeyPair>& ExtraSettings, FName SettingName, EBlueprintResultSwitch &Result, int32& OutIndex)
{
	OutIndex = ExtraSettings.IndexOfByPredicate([&](const FSessionPropertyKeyPair& it) {return it.Key == SettingName; });

	Result = OutIndex != INDEX_NONE ? EBlueprintResultSwitch::OnSuccess : EBlueprintResultSwitch::OnFailure;
}	

void UChineseSessionsLibrary::AddOrModifyExtraSettings(UPARAM(ref) TArray<FSessionPropertyKeyPair> & SettingsArray, UPARAM(ref) TArray<FSessionPropertyKeyPair> & NewOrChangedSettings, TArray<FSessionPropertyKeyPair> & ModifiedSettingsArray)
{
	ModifiedSettingsArray = SettingsArray;

	bool bFoundSetting = false;
	// For each new setting
	for (const FSessionPropertyKeyPair& Setting : NewOrChangedSettings)
	{
		bFoundSetting = false;

		for (FSessionPropertyKeyPair & itr : ModifiedSettingsArray)
		{
			// Manually comparing the keys
			if (itr.Key == Setting.Key)
			{
				bFoundSetting = true;
				itr.Data = Setting.Data;
			}
		}

		// If it was not found, add to the array instead
		if (!bFoundSetting)
		{
			ModifiedSettingsArray.Add(Setting);
		}
	}

}

void UChineseSessionsLibrary::GetExtraSettings(FBlueprintSessionResult SessionResult, TArray<FSessionPropertyKeyPair> & ExtraSettings)
{
	FSessionPropertyKeyPair NewSetting;
	for (auto& Elem : SessionResult.OnlineResult.Session.SessionSettings.Settings)
	{
		NewSetting.Key = Elem.Key;
		NewSetting.Data = Elem.Value.Data;
		ExtraSettings.Add(NewSetting);
	}
}

void UChineseSessionsLibrary::GetSessionState(UObject* WorldContextObject, EBPOnlineSessionState &SessionState)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);

	if (!SessionInterface.IsValid())
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("获取会话状态： 无法获取会话接口！"));
		return;
	}

	SessionState = ((EBPOnlineSessionState)SessionInterface->GetSessionState(NAME_GameSession));
}

void UChineseSessionsLibrary::GetSessionSettings(UObject* WorldContextObject, int32 &NumConnections, int32 &NumPrivateConnections, bool &bIsLAN, bool &bIsDedicated, bool &bAllowInvites, bool &bAllowJoinInProgress, bool &bIsAnticheatEnabled, int32 &BuildUniqueID, TArray<FSessionPropertyKeyPair> &ExtraSettings, EBlueprintResultSwitch &Result)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);

	if (!SessionInterface.IsValid())
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("获取会话设置： 无法获取会话接口！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	FOnlineSessionSettings* settings = SessionInterface->GetSessionSettings(NAME_GameSession);
	if (!settings)
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("获取会话设置： 无法获取会话设置！"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	BuildUniqueID = settings->BuildUniqueId;
	NumConnections = settings->NumPublicConnections;
	NumPrivateConnections = settings->NumPrivateConnections;
	bIsLAN = settings->bIsLANMatch;
	bIsDedicated = settings->bIsDedicated;
	bIsAnticheatEnabled = settings->bAntiCheatProtected;
	bAllowInvites = settings->bAllowInvites;
	bAllowJoinInProgress = settings->bAllowJoinInProgress;

	FSessionPropertyKeyPair NewSetting;

	for (auto& Elem : settings->Settings)
	{
		NewSetting.Key = Elem.Key;
		NewSetting.Data = Elem.Value.Data;
		ExtraSettings.Add(NewSetting);
	}

	Result = EBlueprintResultSwitch::OnSuccess;
}

void UChineseSessionsLibrary::IsPlayerInSession(UObject* WorldContextObject, const FBPUniqueNetId &PlayerToCheck, bool &bIsInSession)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);

	if (!SessionInterface.IsValid())
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("正在播放会话： 无法获取会话接口！"));
		bIsInSession = false;
		return;
	}

	bIsInSession = SessionInterface->IsPlayerInSession(NAME_GameSession, *PlayerToCheck.GetUniqueNetId());
}

FSessionsSearchSetting UChineseSessionsLibrary::MakeLiteralSessionSearchProperty(FSessionPropertyKeyPair SessionSearchProperty, EOnlineComparisonOpRedux ComparisonOp)
{
	FSessionsSearchSetting setting;
	setting.PropertyKeyPair = SessionSearchProperty;
	setting.ComparisonOp = ComparisonOp;

	return setting;
}

FSessionPropertyKeyPair UChineseSessionsLibrary::MakeLiteralSessionPropertyByte(FName Key, uint8 Value)
{
	FSessionPropertyKeyPair Prop;
	Prop.Key = Key;
	Prop.Data.SetValue((int32)Value);
	return Prop;
}

FSessionPropertyKeyPair UChineseSessionsLibrary::MakeLiteralSessionPropertyBool(FName Key, bool Value)
{
	FSessionPropertyKeyPair Prop;
	Prop.Key = Key;
	Prop.Data.SetValue(Value);
	return Prop;
}

FSessionPropertyKeyPair UChineseSessionsLibrary::MakeLiteralSessionPropertyString(FName Key, FString Value)
{
	FSessionPropertyKeyPair Prop;
	Prop.Key = Key;
	Prop.Data.SetValue(Value);
	return Prop;
}

FSessionPropertyKeyPair UChineseSessionsLibrary::MakeLiteralSessionPropertyInt(FName Key, int32 Value)
{
	FSessionPropertyKeyPair Prop;
	Prop.Key = Key;
	Prop.Data.SetValue(Value);
	return Prop;
}

FSessionPropertyKeyPair UChineseSessionsLibrary::MakeLiteralSessionPropertyFloat(FName Key, float Value)
{
	FSessionPropertyKeyPair Prop;
	Prop.Key = Key;
	Prop.Data.SetValue(Value);
	return Prop;
}

void UChineseSessionsLibrary::GetSessionPropertyByte(const TArray<FSessionPropertyKeyPair> & ExtraSettings, FName SettingName, ESessionSettingSearchResult &SearchResult, uint8 &SettingValue)
{
	for (FSessionPropertyKeyPair itr : ExtraSettings)
	{
		if (itr.Key == SettingName)
		{
			if (itr.Data.GetType() == EOnlineKeyValuePairDataType::Int32)
			{
				int32 Val;
				itr.Data.GetValue(Val);
				SettingValue = (uint8)(Val);
				SearchResult = ESessionSettingSearchResult::Found;
			}
			else
			{
				SearchResult = ESessionSettingSearchResult::WrongType;
			}
			return;
		}
	}

	SearchResult = ESessionSettingSearchResult::NotFound;
	return;
}

void UChineseSessionsLibrary::GetSessionPropertyBool(const TArray<FSessionPropertyKeyPair> & ExtraSettings, FName SettingName, ESessionSettingSearchResult &SearchResult, bool &SettingValue)
{
	for (FSessionPropertyKeyPair itr : ExtraSettings)
	{
		if (itr.Key == SettingName)
		{
			if (itr.Data.GetType() == EOnlineKeyValuePairDataType::Bool)
			{
				itr.Data.GetValue(SettingValue);
				SearchResult = ESessionSettingSearchResult::Found;
			}
			else
			{
				SearchResult = ESessionSettingSearchResult::WrongType;
			}
			return;
		}
	}

	SearchResult = ESessionSettingSearchResult::NotFound;
	return;
}

void UChineseSessionsLibrary::GetSessionPropertyString(const TArray<FSessionPropertyKeyPair> & ExtraSettings, FName SettingName, ESessionSettingSearchResult &SearchResult, FString &SettingValue)
{
	for (FSessionPropertyKeyPair itr : ExtraSettings)
	{
		if (itr.Key == SettingName)
		{
			if (itr.Data.GetType() == EOnlineKeyValuePairDataType::String)
			{
				itr.Data.GetValue(SettingValue);
				SearchResult = ESessionSettingSearchResult::Found;
			}
			else
			{
				SearchResult = ESessionSettingSearchResult::WrongType;
			}
			return;
		}
	}

	SearchResult = ESessionSettingSearchResult::NotFound;
	return;
}

void UChineseSessionsLibrary::GetSessionPropertyInt(const TArray<FSessionPropertyKeyPair> & ExtraSettings, FName SettingName, ESessionSettingSearchResult &SearchResult, int32 &SettingValue)
{
	for (FSessionPropertyKeyPair itr : ExtraSettings)
	{
		if (itr.Key == SettingName)
		{
			if (itr.Data.GetType() == EOnlineKeyValuePairDataType::Int32)
			{
				itr.Data.GetValue(SettingValue);
				SearchResult = ESessionSettingSearchResult::Found;
			}
			else
			{
				SearchResult = ESessionSettingSearchResult::WrongType;
			}
			return;
		}
	}

	SearchResult = ESessionSettingSearchResult::NotFound;
	return;
}

void UChineseSessionsLibrary::GetSessionPropertyFloat(const TArray<FSessionPropertyKeyPair> & ExtraSettings, FName SettingName, ESessionSettingSearchResult &SearchResult, float &SettingValue)
{
	for (FSessionPropertyKeyPair itr : ExtraSettings)
	{
		if (itr.Key == SettingName)
		{
			if (itr.Data.GetType() == EOnlineKeyValuePairDataType::Float)
			{
				itr.Data.GetValue(SettingValue);
				SearchResult = ESessionSettingSearchResult::Found;
			}
			else
			{
				SearchResult = ESessionSettingSearchResult::WrongType;
			}
			return;
		}
	}

	SearchResult = ESessionSettingSearchResult::NotFound;
	return;
}


bool UChineseSessionsLibrary::HasOnlineSubsystem(FName SubSystemName)
{
	return IOnlineSubsystem::DoesInstanceExist(SubSystemName);
}

void UChineseSessionsLibrary::GetNetPlayerIndex(APlayerController *PlayerController, int32 &NetPlayerIndex)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("获取网络玩家索引： 收到了一个错误的玩家控制器!"));
		NetPlayerIndex = 0;
		return;
	}

	NetPlayerIndex = PlayerController->NetPlayerIndex;
	return;
}

void UChineseSessionsLibrary::UniqueNetIdToString(const FBPUniqueNetId& UniqueNetId, FString &String)
{
	const FUniqueNetId * ID = UniqueNetId.GetUniqueNetId();

	if ( !ID )
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("字符串的唯一网络Id： 收到了一个错误的 网络唯一ID！"));
		String = "ERROR, BAD UNIQUE NET ID";
	}
	else
		String = ID->ToString();
}


void UChineseSessionsLibrary::GetUniqueNetID(APlayerController *PlayerController, FBPUniqueNetId &UniqueNetId)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("从控制器获取唯一网络Id 收到了一个错误的 PlayerController!"));
		return;
	}

	if (APlayerState* PlayerState = (PlayerController != NULL) ? PlayerController->PlayerState : NULL)
	{
		UniqueNetId.SetUniqueNetId(PlayerState->GetUniqueId().GetUniqueNetId());
		if (!UniqueNetId.IsValid())
		{
			UE_LOG(ChineseSessionsLog, Warning, TEXT("从控制器获取唯一网络Id couldn't get the player 网络唯一ID！"));
		}
		return;
	}
}

void UChineseSessionsLibrary::GetUniqueNetIDFromPlayerState(APlayerState *PlayerState, FBPUniqueNetId &UniqueNetId)
{
	if (!PlayerState)
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("从玩家状态获取唯一网络Id： 收到了一个错误的 玩家状态!"));
		return;
	}

	UniqueNetId.SetUniqueNetId(PlayerState->GetUniqueId().GetUniqueNetId());
	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("从玩家状态获取唯一网络Id： 无法获取玩家网络唯一ID！"));
	}
	return;
}

bool UChineseSessionsLibrary::IsValidUniqueNetID(const FBPUniqueNetId &UniqueNetId)
{
	return UniqueNetId.IsValid();
}

bool UChineseSessionsLibrary::EqualEqual_UNetIDUnetID(const FBPUniqueNetId &A, const FBPUniqueNetId &B)
{	
	return ((A.IsValid() && B.IsValid()) && (*A.GetUniqueNetId() == *B.GetUniqueNetId()));
}

void UChineseSessionsLibrary::SetPlayerName(APlayerController *PlayerController, FString PlayerName)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("从控制器设置本地玩家名称： 错误的玩家控制器！"));
		return;
	}

	if (APlayerState* PlayerState = (PlayerController != NULL) ? PlayerController->PlayerState : NULL)
	{
		PlayerState->SetPlayerName(PlayerName);
		return;
	}
	else
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("从控制器设置本地玩家名称： 出现了错误的玩家状态！"));
	}
}

void UChineseSessionsLibrary::GetPlayerName(APlayerController *PlayerController, FString &PlayerName)
{
	if (!PlayerController)
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("从控制器获取本地玩家名称： 错误的玩家控制器！"));
		return;
	}

	if (APlayerState* PlayerState = (PlayerController != NULL) ? PlayerController->PlayerState : NULL)
	{
		PlayerName = PlayerState->GetPlayerName();
		return;
	}
	else
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("从控制器获取本地玩家名称： 出现了错误的玩家状态！"));
	}
}

void UChineseSessionsLibrary::GetNumberOfNetworkPlayers(UObject* WorldContextObject, int32 &NumNetPlayers)
{
	//Get World
	UWorld* TheWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (!TheWorld)
	{
		UE_LOG(ChineseSessionsLog, Warning, TEXT("获取网络玩家数量 获取“世界”失败！"));
		return;
	}

	NumNetPlayers = TheWorld->GetGameState()->PlayerArray.Num();
}

bool UChineseSessionsLibrary::ServerTravel(UObject* WorldContextObject, const FString& FURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	if (!WorldContextObject)
	{
		return false;
	}

	//using a context object to get the world
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (World)
	{
		return World->ServerTravel(FURL, bAbsolute, bShouldSkipGameNotify);
	}

	return false;
}