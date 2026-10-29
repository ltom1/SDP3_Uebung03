#include "Song.h"

Song::Song(std::string const& name, size_t const duration) : 
	mName{name},
	mDuration{duration}{

}

std::string Song::GetName() const
{
	return mName;
}

size_t Song::GetDuration() const
{
	return mDuration;
}

void Song::SetName(std::string const& name)
{
	mName = name;
}
std::string Song::ToString() const
{
	return mName + " (" + std::to_string(mDuration) + " sec)";
}