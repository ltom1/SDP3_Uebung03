#include "VideoPlayerAdapter.h"

#include <iostream>

static std::string const cErrNullptr = "Provided player is nullptr";

VideoPlayerAdapter::VideoPlayerAdapter(VideoPlayer::UPtr player)
{
	if (player == nullptr) throw std::invalid_argument{ cErrNullptr };

	mPlayer = std::move(player);
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
	size_t const vol = mPlayer->GetVolume(); // Avoid size_t underflow.
	if (vol >= 1) mPlayer->SetVolume(vol - 1);
}

void VideoPlayerAdapter::Next()
{
	// Wrap around when end is reached.
	if (!mPlayer->Next()) mPlayer->First();
}

void VideoPlayerAdapter::Prev()
{
	size_t const curIdx = mPlayer->ActIndex();
	if (curIdx == 0) {
		while (mPlayer->Next()) {}
	}
	else
	{
		mPlayer->First();
		for (size_t i = 0; i < curIdx - 1; ++i)
		{
			mPlayer->Next();
		}
	}

}

void VideoPlayerAdapter::Select(std::string const& name)
{
	size_t const oldIdx = mPlayer->ActIndex();

	if (mPlayer->First()) // Player has videos.
	{
		do
		{
			if (mPlayer->ActVideo() == name) // Found!
			{
				mPlayer->Play(); // Play it.
				return; 
			}
		} while (mPlayer->Next()); // Continue until end is reached.

		// Was not found...
		// ... go back to original idx.
		mPlayer->First();
		for (size_t i = 0; i < oldIdx; ++i)
		{
			mPlayer->Next();
		}
	}

	std::cout << "video: " << name << " not found!" << std::endl;
}