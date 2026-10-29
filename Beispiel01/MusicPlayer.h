#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "Object.h"
#include "Song.h"

#include <string>
#include <vector>
#include <memory>

class MusicPlayer : public Object
{
public:
	static constexpr size_t DEFAULT_VOLUME = 15;
	static constexpr size_t MAX_VOLUME = 100;

	/// @brief std::unique_ptr of this class.
	using UPtr = std::unique_ptr<MusicPlayer>;

	//starts playing with the actual song in list
	void Start();
	//stops playing
	void Stop();
	//switches to next song and starts at the end with first song
	void SwitchNext();
	//gets index of actual song
	size_t GetActIndex() const;
	//finds a song by name in playlist
	bool Find(std::string const& name);
	//gets count of songs in playlist
	size_t GetCount() const;

	//increases the volume relative to the actual volume
	void IncreaseVol(size_t const vol);
	//decreases the volume relative to the actual volume
	void DecreaseVol(size_t const vol);
	//adds a song to playlist
	void Add(std::string const& name, size_t const dur);

private:
	size_t mVolume = MusicPlayer::DEFAULT_VOLUME;
	std::vector<Song> mSongs;
};

#endif