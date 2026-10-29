#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "Object.h"
#include "Video.h"

#include <string>
#include <vector>
#include <memory>


/// @author Thomas Lechner
/// @date 29.10.2026
/// @brief A video player for managing videos.
/// 
/// Holds multiple Video objects and lets you switch between them and play/stop them. Can adjust the volume of the video currently playing.
class VideoPlayer : public Object
{
public:
	/// @brief The default value for the volume.
	static constexpr size_t DEFAULT_VOLUME = 8;
	/// @brief The maximum value for the volume.
	static constexpr size_t MAX_VOLUME = 50;

	/// @brief std::unique_ptr of this class.
	using UPtr = std::unique_ptr<VideoPlayer>;

	/// @brief Starts playing the currently selected Video.
	void Play() const;
	/// @brief Stops playing the currently playing Video.
	void Stop() const;

	/// @brief Switches to the first Video in the playlist.
	/// @returns false, when there are no Videos in the playlist; otherwise true.
	bool First();
	/// @brief Switches to the next Video in the playlist.
	/// @returns false, when the last song is reached, otherwise true.
	bool Next();

	/// @brief Get currently selected Video's index.
	/// @returns The index of the currently selected Video.
	size_t ActIndex() const;
	/// @brief Get currently selected Video's name.
	/// @returns The name of the currently selected Video.
	std::string const ActVideo() const;

	/// @brief Sets the volume.
	/// @param[in] vol The volume to change to.
	/// 
	/// If the provided volume is bigger than MAX_VOLUME, it is clamped to MAX_VOLUME.
	void SetVolume(size_t const vol);
	/// @brief Gets the volume.
	/// @returns The current volume.
	size_t GetVolume() const;

	/// @brief Adds a Video to the playlist.
	/// @param[in] name The name of the video.
	/// @param[in] dur The duration of the video in minutes.
	/// @param[in] format The video's VideoFormat.
	void Add(std::string const& name, size_t const dur, VideoFormat const& format);

private:
	/// @brief The current volume.
	size_t mVolume = VideoPlayer::DEFAULT_VOLUME;

	/// @brief The playlist. 
	std::vector<Video> mVideos;

	/// @brief The index of the currently selected Video.
	size_t mCurIdx = 0;
};

#endif