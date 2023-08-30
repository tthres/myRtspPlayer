#pragma once

#include <atomic>
#include <thread>

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "StreamClientState.h"
#include "DummySink.h"
#include "ourRTSPClient.h"

class RTSPClientWrapper
{
public:
    RTSPClientWrapper();
    ~RTSPClientWrapper();

    void usage(char const *progName);
    bool play(char const *progName, char const *rtspURL);


private:
    void Start();
    void Stop();

    TaskScheduler *scheduler;
    UsageEnvironment *env;

    std::unique_ptr<IDecoder> _decoder;
    std::unique_ptr<IRender> _render;

    std::atomic_bool _run = false;
    std::string _url;
    std::string _progName;

    std::thread _pullStreamThread;
    std::thread _decodeThread;

    char eventLoopWatchVariable;
};