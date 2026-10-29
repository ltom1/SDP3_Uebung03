#include "VideoPlayer.h"

#include <iostream>
#include <cassert>

// Player output.
static std::string const cNoVideos = "no video in playlist!";

void VideoPlayer::Play() const
{
	if (mVideos.empty())
	{
		std::cout << cNoVideos << std::endl;
		return;
	}

	std::cout << "playing video number " << mCurIdx + 1 << ": " << mVideos.at(mCurIdx).ToString() << std::endl;
}

void VideoPlayer::Stop() const
{
	if (mVideos.empty())
	{
		std::cout << cNoVideos << std::endl;
		return;
	}

	std::cout << "stop video: " << mVideos.at(mCurIdx).ToString() << std::endl;
}

bool VideoPlayer::First()
{
	mCurIdx = 0;
	return mVideos.empty();
}

bool VideoPlayer::Next()
{
	// No videos left?
	if (mCurIdx >= mVideos.size() - 1) return false;

	++mCurIdx;
	return true;
}

size_t VideoPlayer::ActIndex() const
{
	return mCurIdx;
}

std::string const VideoPlayer::ActVideo() const
{
	return mVideos.at(mCurIdx).GetName();
}

void VideoPlayer::SetVolume(size_t const vol)
{
	mVolume = vol % VideoPlayer::MAX_VOLUME;
}

size_t VideoPlayer::GetVolume() const
{
	return mVolume;
}

void VideoPlayer::Add(std::string const& name, size_t const dur, VideoFormat const& format)
{
	mVideos.emplace_back(name, dur, format);
}

