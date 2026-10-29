#include "Video.h"

Video::Video(std::string const& name, size_t const duration, VideoFormat const format) :
	mName{ name },
	mDuration{ duration },
	mFormat{ format }
{
}

std::string Video::GetName() const
{
	return mName;
}

size_t Video::GetDuration() const
{
	return mDuration;
}

VideoFormat Video::GetFormat() const
{
	return mFormat;
}

void Video::SetName(std::string const& name)
{
	mName = name;
}

std::string Video::ToString() const
{
	return mName + " [duration -> " + std::to_string(mDuration) + " min], " + to_string(mFormat);
}