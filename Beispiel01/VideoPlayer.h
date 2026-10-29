#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "Object.h"
#include "Video.h"

#include <string>
#include <vector>
#include <memory>

class VideoPlayer : public Object
{
public:
	static constexpr size_t DEFAULT_VOLUME = 8;
	static constexpr size_t MIN_VOLUME = 0;
	static constexpr size_t MAX_VOLUME = 50;

	/// @brief std::unique_ptr of this class.
	using UPtr = std::unique_ptr<VideoPlayer>;

	//starts playing with the actual video in list
	void Play() const;
	//stops playing
	void Stop() const;
	//switches to first video in playlist and returns true, otherwise false if list is empty.
	bool First();
	//switches to next video in playlist and returns true, otherwise false if last song is reached.
	bool Next();
	//returns index of actual video
	size_t ActIndex() const;
	//returns name of actual video
	std::string const ActVideo() const;
	//sets volume (min volume=0 and max volume=50)
	void SetVolume(size_t const vol);
	//gets actual volume
	size_t GetVolume() const;
	//adds a video to playlist
	void Add(std::string const& name, size_t const dur, VideoFormat const& format);
private:
	size_t mVolume = VideoPlayer::DEFAULT_VOLUME;
	std::vector<Video> mVideos;
};

#endif