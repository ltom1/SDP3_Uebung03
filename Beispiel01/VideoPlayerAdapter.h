#ifndef VIDEOPLAYERADAPTER_H
#define VIDEOPLAYERADAPTER_H

#include "IMediaPlayer.h"
#include "VideoPlayer.h"

/// @author Thomas Lechner
/// @date 29.10.2026
/// @brief IMediaPlayer-Adapter for VideoPlayer.
/// 
/// Adapts a VideoPlayer to the IMediaPlayer interface.
class VideoPlayerAdapter : public Object, public IMediaPlayer
{
public:
	/// @brief CTor.
	/// @param[in] player The VideoPlayer to use.
	VideoPlayerAdapter(VideoPlayer::UPtr player);

	/// @brief Play the current Video.
	void Play() override;
	/// @brief Stop the current Video.
	void Stop() override;

	/// @brief Increase the volume by 1.
	void VolInc() override;
	/// @brief Decrease the volume by 1.
	void VolDec() override;

	/// @brief Select the next Video.
	///
	/// Wraps around when the end is reached.
	void Next() override;
	/// @brief Select the previous Video.
	///
	/// Does not wrap around when the beginning is reached.
	void Prev() override;
	/// @brief Select the Video based on a name.
	/// @param[in] name The name of the Video to select.
	void Select(std::string const& name) override;

private:
	/// @brief The VideoPlayer to use.
	VideoPlayer::UPtr mPlayer;
};

#endif