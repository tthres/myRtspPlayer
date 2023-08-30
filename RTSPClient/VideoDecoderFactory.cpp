#include "VideoDecoderFactory.h"
#include "FFmpegDecoder.h"

std::unique_ptr<IDecoder> VideoDecoderFactory::CreateDecoder(DecoderType type)
{
	if (type == DecoderType::FFmpeg) {
		// return std::make_unique<FFmpegDecoder>(std::make_unique<FFmpegDecoder>());
		return std::make_unique<FFmpegDecoder>();
	}
	else if (type == DecoderType::VLC) {
		// return std::make_unique<VideoDecoder>(std::make_unique<VLCDecoder>());
	}
	// ... other cases
	return nullptr;
}