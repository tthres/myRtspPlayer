#include <iostream>
#include <fstream>

#include "FFmpegDecoder.h"

FFmpegDecoder::FFmpegDecoder()
	: _codec_ctx(nullptr)
	, _av_codec(nullptr)
	, _av_frame(nullptr)
	, _sws_ctx(nullptr)
	, _av_parser(nullptr)
	, _av_packet(nullptr)
{
	Open("");
}

FFmpegDecoder::~FFmpegDecoder()
{
	Close();
}

bool FFmpegDecoder::Open(const std::string& filepath)
{
	avcodec_register_all();

	_av_codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!_av_codec) {
		std::cerr << "avcodec_find_decoder failed." << std::endl;
		return false;
	}

	_codec_ctx = avcodec_alloc_context3(_av_codec);
	if (!_codec_ctx) {
		std::cerr << "avcodec_alloc_context3 failed." << std::endl;
		return false;
	}

	if (avcodec_open2(_codec_ctx, _av_codec, nullptr) < 0) {
		std::cerr << "avcodec_open2 failed." << std::endl;
		return false;
	}

	_av_frame = av_frame_alloc();
	if (!_av_frame) {
		std::cerr << "av_frame_alloc failed." << std::endl;
		return false;
	}

	_av_packet = av_packet_alloc();
	if (!_av_packet) {
		std::cerr << "av_packet_alloc failed." << std::endl;
		return false;
	}

	_av_parser = av_parser_init(AV_CODEC_ID_H264);
	if (!_av_parser) {
		std::cerr << "av_parser_init failed." << std::endl;
		return false;
	}

	return true;
}

bool FFmpegDecoder::Decode(uint8_t* packet_data, uint32_t packet_length, YUVFrame& frame)
{
	av_frame_unref(_av_frame);

	if (packet_length > 0)
	{
#ifdef READ_FROM_FILE



		int bytesParsed = av_parser_parse2(_av_parser, 
										_codec_ctx, 
										&_av_packet->data, 
										&_av_packet->size, 
										packet_data, 
										packet_length, 
										AV_NOPTS_VALUE, 
										AV_NOPTS_VALUE, 
										0);
		if (bytesParsed < 0) {
			std::cerr << "Error while parsing" << std::endl;
			return false;
		}

		// ÏòÇ°ÒÆÎ»
		packet_data += bytesParsed;
		packet_length -= bytesParsed;
#endif // READ_FROM_FILE

		_av_packet->data = packet_data;
		_av_packet->size = packet_length;

		int ret = avcodec_send_packet(_codec_ctx, _av_packet);
		if (ret < 0) {
			std::cerr << "Error sending a packet for decoding : " <<  std::endl;
			return false;
		}

		// std::ofstream out = std::ofstream("test.yuv", std::ios::binary | std::ios::app);

		while (1)
		{
			ret = avcodec_receive_frame(_codec_ctx, _av_frame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				break;
			}
			else if (ret < 0) {
				std::cerr << "Error during decoding" << std::endl;
				return false;
			}

			std::cout << "width: " << _av_frame->width << " height: " << _av_frame->height << std::endl;

			// TODO : äÖÈ¾

			frame.width = _av_frame->width;
			frame.height = _av_frame->height;

			frame.y.y = _av_frame->data[0];
			frame.y.linesize = _av_frame->linesize[0];

			frame.u.u = _av_frame->data[1];
			frame.u.linesize = _av_frame->linesize[1];

			frame.v.v = _av_frame->data[2];
			frame.v.linesize = _av_frame->linesize[2];

			av_frame_unref(_av_frame);
		}
			
	

	}


	return true;
}


void FFmpegDecoder::Close()
{
	if (_codec_ctx) {
		avcodec_close(_codec_ctx);
		avcodec_free_context(&_codec_ctx);
		_codec_ctx = nullptr;
	}
	if (_av_frame) {
		av_frame_free(&_av_frame);
		_av_frame = nullptr;
	}
	if (_sws_ctx) {
		sws_freeContext(_sws_ctx);
		_sws_ctx = nullptr;
	}
	if (_av_parser) {
		av_parser_close(_av_parser);
		_av_parser = nullptr;
	}
	if (_av_packet) {
		av_packet_free(&_av_packet);
		_av_packet = nullptr;
	}
	if (_av_codec) {
		_av_codec = nullptr;
	}
}
