#include "MusicPlayer.h"

#include <iostream>

static std::string const cNoSongs = "no song in playlist!";

void MusicPlayer::Start()
{
	if (mSongs.empty()){
		std::cout << cNoSongs << std::endl;
	}
	else
	{
		std::cout << "playing song number " << mActIndex + 1 << ": " << mSongs.at(mActIndex).ToString() << std::endl;
	}
}
void MusicPlayer::Stop() {
	if (mSongs.empty()) {
		std::cout << cNoSongs << std::endl;
	}
	else
	{
		std::cout << "stop song: " << mSongs.at(mActIndex).ToString() << std::endl;
	}
}

void MusicPlayer::SwitchNext()
{
	size_t numOfSongs = mSongs.size();
	if (numOfSongs > 1)
	{
		// set index to first song if actIndex is at last one
		mActIndex = (mActIndex + 1) % numOfSongs; 
	}
}

size_t MusicPlayer::GetActIndex() const
{
	return mActIndex;
}

bool MusicPlayer::Find(std::string const& name)
{
	for (size_t i = 0; i < mSongs.size(); i++) {
		if (mSongs.at(i).GetName() == name)
		{
			mActIndex = i;
			return true;
		}
	}
	return false;
}

size_t MusicPlayer::GetCount() const
{
	return mSongs.size();
}

void MusicPlayer::IncreaseVol(size_t const vol)
{
	mVolume = mVolume + vol;
	if (mVolume > MAX_VOLUME) mVolume = MAX_VOLUME;
}

void MusicPlayer::DecreaseVol(size_t const vol)
{
	if (vol > mVolume) // prevent size_t underflow
	{
		mVolume = 0;
	}
	else
	{
		mVolume = mVolume - vol;
	}
}

void MusicPlayer::Add(std::string const& name, size_t const dur)
{
	mSongs.emplace_back(name, dur);
}





