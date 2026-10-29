#ifndef SONG_H
#define SONG_H

#include "Object.h"

#include <string>


/// @author Philip Ackermann
/// @date 29.10.2026
/// @brief A song.
/// 
/// Holds information about a song: name and duration in seconda.
class Song : public Object
{
public:
	/// @brief CTor.
	/// @param[in] name The name of the song.
	/// @param[in] duration The duration of the song in seconds.
	/// 
	/// Initializes the song class.
	Song(std::string const& name, size_t const duration);

	/// @brief Getter for name.
	/// @returns The name of the song.
	std::string GetName() const;

	/// @brief Getter for duration.
	/// @returns The duration of the song in seconds.
	size_t GetDuration() const;

	/// @brief Setter for name.
	/// @param[in] name The new name of the song.
	void SetName(std::string const& name);

	/// @brief String representation of the song.
	/// @returns The song as a formated string.
	std::string ToString() const;
private:
	/// @brief The name of the song.
	std::string mName;
	/// @brief The duration of the song in seconds.
	size_t const mDuration;
};

#endif