#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "Object.h"
#include "Song.h"

#include <string>
#include <vector>
#include <memory>

/// @author Philip Ackermann
/// @date 29.10.2026
/// @brief A music player for managing songs.
/// 
/// Holds multiple Song objects and lets you switch between them and play/stop them. Can adjust the volume of the song currently playing.
class MusicPlayer : public Object
{
public:
	/// @brief The default value for the volume.
	static constexpr size_t DEFAULT_VOLUME = 15;
	
	/// @brief The maximum value for the volume.
	static constexpr size_t MAX_VOLUME = 100;

	/// @brief std::unique_ptr of this class.
	using UPtr = std::unique_ptr<MusicPlayer>;

	/// @brief Starts playing the currently selected Song.
	void Start();
	
	/// @brief Stops playing the currently playing Song.
	void Stop();
	
	/// @brief Switches to the next Song in the playlist.
	void SwitchNext();
	
	/// @brief Get currently selected Song's index.
	/// @returns The index of the currently selected Song.
	size_t GetActIndex() const;
	
	/// @param[in] name Name of the song to look for.
	/// @returns If Song with name exists in playlist.
	bool Find(std::string const& name);
	
	/// @returns The number of songs in playlist.
	size_t GetCount() const;

	/// @brief Increases the volume.
	/// @param[in] vol The value to increase the volume.
	/// 
	/// If the increased volume is higher than MAX_VOLUME, it is clamped to MAX_VOLUME.
	void IncreaseVol(size_t const vol);
	
	/// @brief Decreases the volume.
	/// @param[in] vol The value to decrease the volume.
	/// 
	/// If the decreased volume is lower than 0, it is clamped to 0.
	void DecreaseVol(size_t const vol);
	
	/// @brief Adds a Song to the playlist.
	/// @param[in] name The name of the Song.
	/// @param[in] dur The duration of the Song in seconds.
	void Add(std::string const& name, size_t const dur);

private:
	/// @brief The current volume.
	size_t mVolume = MusicPlayer::DEFAULT_VOLUME;

	/// @brief The playlist. 
	std::vector<Song> mSongs;

	/// @brief The index of the currently selected Song.
	size_t mActIndex = 0;
};

#endif