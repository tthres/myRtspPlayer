#include "DummySink.h"

extern FrameBuffer globalFrameBuffer;
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 100000

bool firstFrame = true;

DummySink* DummySink::createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId) {
    return new DummySink(env, subsession, streamId);
}

DummySink::DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
  : MediaSink(env),
    fSubsession(subsession)/*,
    _decoder(VideoDecoderFactory::CreateDecoder(DecoderType::FFmpeg)),
    _render(RenderFactory::CreateRender(RenderType::D3D)) */
{
    fStreamId = strDup(streamId);
    fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
    fReceiveBuffer[0] = 0x00;
    fReceiveBuffer[1] = 0x00;
    fReceiveBuffer[2] = 0x00;
    fReceiveBuffer[3] = 0x01;
    _buf = new std::vector<unsigned char>;
}

DummySink::~DummySink() {
    delete[] fReceiveBuffer;
    delete[] fStreamId;
    delete _buf;
}

void DummySink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned durationInMicroseconds) {
    DummySink* sink = (DummySink*)clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

// If you don't want to see debugging output for each received frame, then comment out the following line:
#define DEBUG_PRINT_EACH_RECEIVED_FRAME 0

void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {
  // We've just received a frame of data.  (Optionally) print out information about it:
#ifdef DEBUG_PRINT_EACH_RECEIVED_FRAME
    if (fStreamId != NULL) envir() << "Stream \"" << fStreamId << "\"; ";
    envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
    if (numTruncatedBytes > 0) envir() << " (with " << numTruncatedBytes << " bytes truncated)";
    char uSecsStr[6+1]; // used to output the 'microseconds' part of the presentation time
    sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
    envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;
    if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
        envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
    }
#ifdef DEBUG_PRINT_NPT
  envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
#endif
  envir() << "\n";
#endif
  
    if(!strcmp(fSubsession.mediumName(), "video"))
    {
        auto sample = globalFrameBuffer.CreateFrameSample(frameSize + 4, FrameSample::FrameType::H264);
        if (sample)
        {
            memcpy(sample->GetDataPointer(), fReceiveBuffer, frameSize + 4);
            globalFrameBuffer.PushFrame(sample);
		}

#if 0
        if(firstFrame)
        {
            unsigned int num;
            SPropRecord *sps = parseSPropParameterSets(fSubsession.fmtp_spropparametersets(), num);
            // For H.264 video stream, we use a special sink that insert start_codes:
            struct timeval tv= {0,0};
            unsigned char start_code[4] = {0x00, 0x00, 0x00, 0x01};

            _buf->insert(_buf->end(), start_code, start_code + 4);
            _buf->insert(_buf->end(), sps[0].sPropBytes, sps[0].sPropBytes + sps[0].sPropLength);
            _buf->insert(_buf->end(), start_code, start_code + 4);
            _buf->insert(_buf->end(), sps[1].sPropBytes, sps[1].sPropBytes + sps[1].sPropLength);

            delete [] sps;


            firstFrame = False;
        }


        char *pbuf = (char *)fReceiveBuffer;
        char head[4] = {0x00, 0x00, 0x00, 0x01};

        _buf->insert(_buf->end(), head, head + 4);
        _buf->insert(_buf->end(), pbuf, pbuf + frameSize);
        
        // TODO: decode h264, and render to screen
        YUVFrame yuvFrame;

        if(_decoder->Decode(_buf->data(), _buf->size(), yuvFrame))
        {
            _render->Render(yuvFrame);
            _buf->clear();
        }

#endif    
    }


    // Then continue, to request the next frame of data:
    continuePlaying();
}



Boolean DummySink::continuePlaying() {
    if (fSource == NULL) return False; // sanity check (should not happen)

    // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
    fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
                            afterGettingFrame, this,
                            onSourceClosure, this);
    return True;
}