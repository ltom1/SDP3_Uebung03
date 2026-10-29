#ifndef VIDEOPLAYERADAPTER_H
#define VIDEOPLAYERADAPTER_H

#include "IMediaPlayer.h"
#include "VideoPlayer.h"


class VideoPlayerAdapter : public Object, public IMediaPlayer
{
public:
	VideoPlayerAdapter(VideoPlayer::UPtr player);

	void Play() override;
	void VolInc() override;
	void VolDec() override;
	void Stop() override;
	void Next() override;
	void Prev() override;
	void Select(std::string const& name) override;
private:
	VideoPlayer::UPtr mPlayer;
};

#endif