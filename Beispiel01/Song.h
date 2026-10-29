#ifndef SONG_H
#define SONG_H

#include "Object.h"

class Song : public Object
{
public:
	Song(std::string const& name, size_t const duration);

	std::string GetName() const;
	size_t GetDuration() const;

	void SetName(std::string const& name);
private:
	std::string mName;
	size_t const mDuration;
};

#endif