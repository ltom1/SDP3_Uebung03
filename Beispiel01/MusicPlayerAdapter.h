#ifndef MUSICPLAYERADAPTER_H
#define MUSICPLAYERADAPTER_H

#include "IMediaPlayer.h"
#include "MusicPlayer.h"

/// @author Philip Ackermann
/// @date 29.10.2026
/// @brief IMediaPlayer-Adapter for MusicPlayer.
/// 
/// Adapts a MusicPlayer to the IMediaPlayer interface.
class MusicPlayerAdapter : public Object, public IMediaPlayer
{
public:
	/// @brief CTor.
	/// @param[in] player The MusicPlayer to use.
	MusicPlayerAdapter(MusicPlayer::UPtr player);


	/// @brief Play the current Song.
	void Play() override;
	/// @brief Stop the current Song.
	void Stop() override;

	/// @brief Increase the volume by 1.
	void VolInc() override;
	/// @brief Decrease the volume by 1.
	void VolDec() override;

	/// @brief Select the next Song.
	///
	/// Wraps around when the end is reached.
	void Next() override;
	/// @brief Select the previous Song.
	///
	/// Wraps around when the beginning is reached.
	void Prev() override;
	/// @brief Select the Song based on a name.
	/// @param[in] name The name of the Song to select.
	void Select(std::string const& name) override;

private:
	/// @brief The MusicPlayer to use.
	MusicPlayer::UPtr mPlayer;
};

#endif