#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <string>
#include <memory>


/// @author Thomas Lechner
/// @date 29.10.2026
/// @brief Media player interface.
/// 
/// Interface used to operate on a media player (volume control, play, stop, select).
class IMediaPlayer
{
public:
	/// @brief std::unique_ptr of this class.
	using UPtr = std::unique_ptr<IMediaPlayer>;

	/// @brief Play the current media.
	virtual void Play() = 0;
	/// @brief Stop the current media.
	virtual void Stop() = 0;

	/// @brief Increase the volume by 1.
	virtual void VolInc() = 0;
	/// @brief Decrease the volume by 1.
	virtual void VolDec() = 0;

	/// @brief Select the next media.
	virtual void Next() = 0;
	/// @brief Select the previous media.
	virtual void Prev() = 0;
	/// @brief Select the media based on a name.
	/// @param[in] name The name of the media to select.
	virtual void Select(std::string const& name) = 0;

	/// @brief Virtual DTor.
	virtual ~IMediaPlayer() = default;
};

#endif