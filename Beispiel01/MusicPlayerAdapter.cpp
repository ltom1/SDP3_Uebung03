#include "MusicPlayerAdapter.h"
#include <iostream>

MusicPlayerAdapter::MusicPlayerAdapter(MusicPlayer::UPtr player) :
	mPlayer{ std::move(player) }
{
}

void MusicPlayerAdapter::Play()
{
	mPlayer->Start();
}

void MusicPlayerAdapter::Stop()
{
	mPlayer->Stop();
}

void MusicPlayerAdapter::VolInc()
{
	mPlayer->IncreaseVol(1);
}

void MusicPlayerAdapter::VolDec()
{
	mPlayer->DecreaseVol(1);
}

void MusicPlayerAdapter::Next()
{
	mPlayer->SwitchNext();
}

void MusicPlayerAdapter::Prev()
{
	for (int i = mPlayer->GetCount(); i > 1; i--) {
		//go forward size-1 times = go once backwards
		mPlayer->SwitchNext();
	}
}

void MusicPlayerAdapter::Select(std::string const& name)
{
	if (!mPlayer->Find(name)) std::cout << "song: " << name << " not found!" << std::endl;
}