#ifndef PLAYERCLIENT_H
#define PLAYERCLIENT_H

#include "Object.h"
#include "IMediaPlayer.h"


/// @author Thomas Lechner
/// @date 03.11.2026
/// @brief A client of the IMediaPlayer interface.
///
/// Tests multiple players by using their IMediaPlayer interface. Tests all IMediaPlayer methods.
class PlayerClient : public Object
{
public:
	/// @brief CTor.
	/// @param[in] player A UPtr to the IMediaPlayer to test.
	PlayerClient(IMediaPlayer::UPtr player);

	/// @brief Changes the IMediaPlayer the client operates on.
	/// @param[in] player A UPtr to the IMediaPlayer to test.
	void SetPlayer(IMediaPlayer::UPtr player);

	/// @brief Run all tests for the selected IMediaPlayer.
	void Run();

private:
	/// @brief The IMediaPlayer under test.
	IMediaPlayer::UPtr mPlayer;
};

#endif