// #pragma once
// #include "DummySink.h"

// class DummyVideoSink : public DummySink
// {
// public:
//     DummyVideoSink(
//         UsageEnvironment& env,
//         MediaSubsession& subsession,
//         char const* streamId);
//     ~DummyVideoSink();

// private:
//     virtual void afterGettingFrame(
//         unsigned frameSize,
//         unsigned numTruncatedBytes,
//         struct timeval presentationTime,
//         unsigned durationInMicroseconds,
//         bool bCorrupted) override;

//     virtual Boolean continuePlaying() override;
    
// };