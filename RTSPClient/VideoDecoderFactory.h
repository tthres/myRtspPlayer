#pragma once

#include <memory>

#include "IDecoder.h"

class VideoDecoderFactory
{
public:
    static std::unique_ptr<IDecoder> CreateDecoder(DecoderType type);
	
};

