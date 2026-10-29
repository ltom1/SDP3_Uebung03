#ifndef VIDEO_H
#define VIDEO_H

#include "VideoFormat.h"

class Video : public Object
{
public:
	Video(std::string const& name, size_t const duration, VideoFormat const format);
	
	std::string GetName() const;
	size_t GetDuration() const;
	VideoFormat GetFormat() const;

	void SetName(std::string const& name);
private:
	std::string mName;
	size_t const mDuration;
	VideoFormat const mFormat;
};

#endif