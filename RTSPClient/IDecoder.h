#pragma once

#include <string>

#include "yuv.h"

enum class DecoderType {
    FFmpeg,
    VLC,
    // ... other decoder types
};

class IDecoder
{
public:

    virtual ~IDecoder() {}

    virtual bool Open(const std::string& filepath) = 0;
    virtual bool Decode(uint8_t* packet_data, uint32_t packet_length, YUVFrame& frame) = 0;
    virtual void Close() = 0;
};

