#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "IDecoder.h"
#include "yuv.h"

class FFmpegDecoder : public IDecoder
{
public:
	FFmpegDecoder();
	virtual ~FFmpegDecoder();

	virtual bool Open(const std::string& filepath);
	virtual bool Decode(uint8_t* packet_data,
						uint32_t packet_length,
						YUVFrame& frame);
	virtual void Close();

private:
	AVCodecContext*			_codec_ctx;
	AVCodec*				_av_codec;
	AVPacket*				_av_packet;
	AVFrame*				_av_frame;  
	SwsContext*				_sws_ctx;
	AVCodecParserContext*	_av_parser;
};

