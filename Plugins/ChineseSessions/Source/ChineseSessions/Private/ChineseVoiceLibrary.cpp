//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023


#include "ChineseVoiceLibrary.h"


//General Log
DEFINE_LOG_CATEGORY(ChineseVoiceLog);

void UChineseVoiceLibrary::IsHeadsetPresent(bool & bHasHeadset, uint8 LocalPlayerNum)
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		bHasHeadset = false;
		UE_LOG(ChineseVoiceLog, Warning, TEXT("检查耳机无法获取语音接口！"));
		return;
	}

	bHasHeadset = VoiceInterface->IsHeadsetPresent(LocalPlayerNum);
}

void UChineseVoiceLibrary::StartNetworkedVoice(uint8 LocalPlayerNum)
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("启动网络语音无法获取语音接口！"));
		return;
	}

	VoiceInterface->StartNetworkedVoice(LocalPlayerNum);
}

void UChineseVoiceLibrary::StopNetworkedVoice(uint8 LocalPlayerNum)
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("停止网络语音无法获取语音接口！"));
		return;
	}

	VoiceInterface->StopNetworkedVoice(LocalPlayerNum);
}

bool UChineseVoiceLibrary::RegisterLocalTalker(uint8 LocalPlayerNum)
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("注册本地对讲机无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->RegisterLocalTalker(LocalPlayerNum);
}

void UChineseVoiceLibrary::RegisterAllLocalTalkers()
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("注册本地对讲机无法获取语音接口！"));
		return;
	}

	VoiceInterface->RegisterLocalTalkers();
}


void UChineseVoiceLibrary::UnRegisterLocalTalker(uint8 LocalPlayerNum)
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("注册本地对讲机无法获取语音接口！"));
		return;
	}

	VoiceInterface->UnregisterLocalTalker(LocalPlayerNum);
}

void UChineseVoiceLibrary::UnRegisterAllLocalTalkers()
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("取消注册所有本地对讲机无法获取语音接口！"));
		return;
	}

	VoiceInterface->UnregisterLocalTalkers();
}

bool UChineseVoiceLibrary::RegisterRemoteTalker(const FBPUniqueNetId& UniqueNetId)
{
	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("注册远程对讲机传递了无效的唯一网络ID！"));
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("注册远程对讲机无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->RegisterRemoteTalker(*UniqueNetId.GetUniqueNetId());
}

bool UChineseVoiceLibrary::UnRegisterRemoteTalker(const FBPUniqueNetId& UniqueNetId)
{
	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("取消注册远程对讲机传递了无效的唯一网络ID！"));
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("取消注册远程对讲机无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->UnregisterRemoteTalker(*UniqueNetId.GetUniqueNetId());
}

void UChineseVoiceLibrary::RemoveAllRemoteTalkers()
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("删除所有远程对讲机无法获取语音接口！"));
		return;
	}

	VoiceInterface->RemoveAllRemoteTalkers();
}

bool UChineseVoiceLibrary::IsLocalPlayerTalking(uint8 LocalPlayerNum)
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("本地玩家正在说话无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->IsLocalPlayerTalking(LocalPlayerNum);
}

bool UChineseVoiceLibrary::IsRemotePlayerTalking(const FBPUniqueNetId& UniqueNetId)
{
	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("远程玩家正在说话传递了无效的唯一网络ID！"));
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("远程玩家正在说话无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->IsRemotePlayerTalking(*UniqueNetId.GetUniqueNetId());
}

bool UChineseVoiceLibrary::IsPlayerMuted(uint8 LocalUserNumChecking, const FBPUniqueNetId& UniqueNetId)
{
	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("玩家被静音传递了无效的唯一网络ID！"));
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("玩家被静音无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->IsMuted(LocalUserNumChecking, *UniqueNetId.GetUniqueNetId());
}

bool UChineseVoiceLibrary::MuteRemoteTalker(uint8 LocalUserNum, const FBPUniqueNetId& UniqueNetId, bool bIsSystemWide)
{
	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("静音远程说话者传递了无效的唯一网络ID！"));
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("静音远程说话者无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->MuteRemoteTalker(LocalUserNum, *UniqueNetId.GetUniqueNetId(), bIsSystemWide);
}

bool UChineseVoiceLibrary::UnMuteRemoteTalker(uint8 LocalUserNum, const FBPUniqueNetId& UniqueNetId, bool bIsSystemWide)
{
	if (!UniqueNetId.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("取消静音远程说话者传递了无效的唯一网络ID！"));
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(ChineseVoiceLog, Warning, TEXT("取消静音远程说话者无法获取语音接口！"));
		return false;
	}

	return VoiceInterface->UnmuteRemoteTalker(LocalUserNum, *UniqueNetId.GetUniqueNetId(), bIsSystemWide);
}


void UChineseVoiceLibrary::GetNumLocalTalkers(int32 & NumLocalTalkers)
{
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface();

	if (!VoiceInterface.IsValid())
	{
		NumLocalTalkers = 0;
		UE_LOG(ChineseVoiceLog, Warning, TEXT("取消静音远程说话者无法获取语音接口！"));
		return;
	}

	NumLocalTalkers = VoiceInterface->GetNumLocalTalkers();
}