#ifndef PLAYERCLIENT_H
#define PLAYERCLIENT_H

#include "Object.h"
#include "IMediaPlayer.h"

class PlayerClient : public Object
{
public:
	PlayerClient(IMediaPlayer::UPtr player);
	void SetPlayer(IMediaPlayer::UPtr player);

	void Run();

private:
	IMediaPlayer::UPtr mPlayer;
};

#endif