#include "VideoPlayerAdapter.h"

#include <iostream>

VideoPlayerAdapter::VideoPlayerAdapter(VideoPlayer::UPtr player) :
	mPlayer{ std::move(player) }
{
}

void VideoPlayerAdapter::Play()
{
	mPlayer->Play();
}

void VideoPlayerAdapter::Stop()
{
	mPlayer->Stop();
}

void VideoPlayerAdapter::VolInc()
{
	mPlayer->SetVolume(mPlayer->GetVolume() + 1);
}

void VideoPlayerAdapter::VolDec()
{
	mPlayer->SetVolume(mPlayer->GetVolume() - 1);
}

void VideoPlayerAdapter::Next()
{
	// Wrap around when end is reached.
	if (!mPlayer->Next()) mPlayer->First();
}

void VideoPlayerAdapter::Prev()
{
	// Get current video index.
	size_t const curIdx = mPlayer->ActIndex();

	if (curIdx == 0) return; // Already at first video or no videos at all.

	mPlayer->First();

	// Switch forward until the correct video is reached.
	for (size_t i = 0; i < curIdx - 1; ++i)
	{
		mPlayer->Next();
	}
}

void VideoPlayerAdapter::Select(std::string const& name)
{
	if (mPlayer->First()) // Player has videos.
	{
		do
		{
			if (mPlayer->ActVideo() == name) return; // Found!
		} while (mPlayer->Next()); // Continue until end is reached.
	}

	std::cout << "video: " + name + " not found!";
}