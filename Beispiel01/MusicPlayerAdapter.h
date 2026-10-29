#ifndef MUSICPLAYERADAPTER_H
#define MUSICPLAYERADAPTER_H

#include "IMediaPlayer.h"
#include "MusicPlayer.h"


class MusicPlayerAdapter : public Object, public IMediaPlayer
{
public:
	MusicPlayerAdapter(MusicPlayer::UPtr player);

	void Play() override;
	void VolInc() override;
	void VolDec() override;
	void Stop() override;
	void Next() override;
	void Prev() override;
	void Select(std::string const& name) override;
private:
	MusicPlayer::UPtr mPlayer;
};

#endif