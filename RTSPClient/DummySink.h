#pragma once

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "DummyVideoSink.h"
#include "VideoDecoderFactory.h"
#include "RenderFactory.h"
#include "FrameBuffer.h"

#include <string>
#include <algorithm>
#include <vector>
#include <memory>

class DummySink: public MediaSink {
public:
  static DummySink* createNew(UsageEnvironment& env,
			      MediaSubsession& subsession, // identifies the kind of data that's being received
			      char const* streamId = NULL); // identifies the stream itself (optional)

private:
    DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);
      // called only by "createNew()"
    virtual ~DummySink();

    static void afterGettingFrame(void* clientData, unsigned frameSize,
                                  unsigned numTruncatedBytes,
          struct timeval presentationTime,
                                  unsigned durationInMicroseconds);
    void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
        struct timeval presentationTime, unsigned durationInMicroseconds);

    FrameTypeState GetFrameTypeState(const unsigned char* data, FrameSample::FrameType codecType);

private:
    
    virtual Boolean continuePlaying();

private:
    u_int8_t* fReceiveBuffer;
    MediaSubsession& fSubsession;
    char* fStreamId;

    std::vector<unsigned char>* _buf; // 使用std::vector作为缓冲区

    

    /*std::unique_ptr<IDecoder> _decoder;
    std::unique_ptr<IRender> _render;*/
};