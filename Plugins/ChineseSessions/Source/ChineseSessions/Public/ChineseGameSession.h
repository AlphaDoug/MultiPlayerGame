//Copyright 2023 Independent Game Author ZhangLiang002. All rights reserved.
//独立游戏作者张亮002版权所有 2023

#pragma once
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerState.h"


#include "ChineseGameSession.generated.h"




/**
 快速包装游戏会话以添加部分禁令实现。仅在当前会话期间禁止
*/
UCLASS(config = Game, notplaceable)
class AChineseGameSession : public AGameSession
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(Transient)
	TMap<FUniqueNetIdRepl, FText> BanList;

	virtual bool BanPlayer(class APlayerController* BannedPlayer, const FText& BanReason)
	{

		if (APlayerState* PlayerState = (BannedPlayer != NULL) ? BannedPlayer->PlayerState : NULL)
		{
			FUniqueNetIdRepl UniqueNetID = PlayerState->GetUniqueId();
			bool bWasKicked = KickPlayer(BannedPlayer, BanReason);

			if (bWasKicked)
			{
				BanList.Add(UniqueNetID, BanReason);
			}

			return bWasKicked;
		}
		
		return false;
	}

	// 这真的应该在请求游戏会话的游戏模式中处理，但我不想强制自定义游戏会话和游戏模式
	//如果在游戏模式中完成，我们可以在实际启动任何玩家信息的ApproveLogin之前进行检查
	virtual void PostLogin(APlayerController* NewPlayer) override
	{
		if (APlayerState* PlayerState = (NewPlayer != NULL) ? NewPlayer->PlayerState : NULL)
		{
			FUniqueNetIdRepl UniqueNetID = PlayerState->GetUniqueId();

			if (BanList.Contains(UniqueNetID))
			{
				KickPlayer(NewPlayer, BanList[UniqueNetID]);
			}
		}
	}
};

AChineseGameSession::AChineseGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}