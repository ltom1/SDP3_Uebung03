#ifndef VIDEO_H
#define VIDEO_H

#include "Object.h"
#include "VideoFormat.h"

#include <string>


/// @author Thomas Lechner
/// @date 29.10.2026
/// @brief A video.
/// 
/// Holds information about a video: name, duration in minutes and the VideoFormat.
class Video : public Object
{
public:
	/// @brief CTor.
	/// @param[in] name The name of the video.
	/// @param[in] duration The duration of the video in minutes.
	/// @param[in] format The video's VideoFormat.
	/// 
	/// Initializes the Video class.
	Video(std::string const& name, size_t const duration, VideoFormat const format);
	
	/// @brief Getter for name.
	/// @returns The name of the video.
	std::string GetName() const;
	/// @brief Getter for duration.
	/// @returns The duration of the video in minutes.
	size_t GetDuration() const;
	/// @brief Getter for format.
	/// @returns The VideoFormat of the video.
	VideoFormat GetFormat() const;

	/// @brief Setter for name.
	/// @param[in] name The new name of the video.
	void SetName(std::string const& name);

	/// @brief String representation of the video.
	/// @returns The video as a formatted string.
	std::string ToString() const;

private:
	/// @brief The name of the video.
	std::string mName;
	/// @brief The duration of the video in minutes.
	size_t mDuration;
	/// @brief The VideoFormat of the video.
	VideoFormat mFormat;
};

#endif