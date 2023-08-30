// #include "DummyVideoSink.h"

// #define NALU_HEADER_SIZE    4
// #define VIDEO_BUFFER_SIZE   (3840 * 2160)

// DummyVideoSink::DummyVideoSink(UsageEnvironment &env, MediaSubsession &subsession, char const *streamId)
//     : DummySink(env, subsession, streamId)
// {
//     // fReceiveBuffer = new u_int8_t[NALU_HEADER_SIZE + VIDEO_BUFFER_SIZE];
//     // fReceiveBuffer[0] = 0x00;
//     // fReceiveBuffer[1] = 0x00;
//     // fReceiveBuffer[2] = 0x00;
//     // fReceiveBuffer[3] = 0x01;
// }

// DummyVideoSink::~DummyVideoSink()
// {
//     // delete[] fReceiveBuffer;
// }

// void DummyVideoSink::afterGettingFrame(
//     unsigned frameSize, 
//     unsigned numTruncatedBytes, 
//     timeval presentationTime, 
//     unsigned durationInMicroseconds, 
//     bool bCorrupted)
// {

//   /*  FILE *_dump = fopen("dump.h264", "ab+");
//     int size = fwrite(fReceiveBuffer, NALU_HEADER_SIZE + frameSize, 1, _dump);
    
//     close(_dump);*/

//     // Then continue, to request the next frame of data:
//     continuePlaying();
// }

// Boolean DummyVideoSink::continuePlaying()
// {
//     //if (fSource == NULL)
//     //    return False; // sanity check (should not happen)

//     //// Request the next frame of data from our input source.  "afterGettingFrame()"
//     //// will get called later, when it arrives:
//     //fSource->getNextFrame(fReceiveBuffer + NALU_HEADER_SIZE, VIDEO_BUFFER_SIZE,
//     //    afterGettingFrame, this,
//     //    onSourceClosure, this);
//     return True;
// }