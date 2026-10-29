#ifndef VIDEOFORMAT_H
#define VIDEOFORMAT_H

#include <string>

/// @author Thomas Lechner
/// @date 29.10.2026
/// @brief Video formats supported by VideoPlayer Video.
enum VideoFormat
{
	WMV,
	AVI,
	MKV
};

/// @brief String representation of a VideoFormat.
/// @param[in] format The VideoFormat.
/// @returns The VideoFormat's string representation.
std::string to_string(VideoFormat const format);

#endif