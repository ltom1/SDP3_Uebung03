#include "VideoFormat.h"

std::string to_string(VideoFormat const format)
{
	switch (format)
	{
		case WMV:	return "WMV-Format";
		case AVI:	return "AVI-Format";
		case MKV:	return "MKV-Format";
	}
	return "Unknown-Format";
}
