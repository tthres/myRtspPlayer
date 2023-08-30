#include "RTSPClientWrapper.h"
#include "FrameBuffer.h"

#define RTSP_CLIENT_VERBOSITY_LEVEL 1 // by default, print verbose output from each "RTSPClient"
extern FrameBuffer globalFrameBuffer;


RTSPClientWrapper::RTSPClientWrapper()
    : eventLoopWatchVariable(0)
    , scheduler(NULL)
    , env(NULL)
    , _run(false)
{
    scheduler = BasicTaskScheduler::createNew();
    env = BasicUsageEnvironment::createNew(*scheduler);
}

RTSPClientWrapper::~RTSPClientWrapper()
{
    env->reclaim();
    delete scheduler;
}

void RTSPClientWrapper::usage(char const *progName)
{
    *env << "Usage: " << progName << " <rtsp-url-1> ... <rtsp-url-N>\n";
    *env << "\t(where each <rtsp-url-i> is a \"rtsp://\" URL)\n";
}

bool RTSPClientWrapper::play(char const *progName, char const *rtspURL)
{
    _url = rtspURL;
    _progName = progName;

    if (_run)
    {
		return false;
	}
    _run = true;
    FrameBuffer _frameBuffer;
    // 创建收流线程
    _pullStreamThread = std::thread([this]
    {
        TaskScheduler* scheduler = BasicTaskScheduler::createNew();
        UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

        RTSPClient* rtspClient = ourRTSPClient::createNew(*env, _url.c_str(), RTSP_CLIENT_VERBOSITY_LEVEL, _progName.c_str());

        if (rtspClient == NULL)
        {
            *env << "Failed to create a RTSP client for URL \"" << _url.c_str() << "\": " << env->getResultMsg() << "\n";
            return false;
        }

        rtspClient->sendDescribeCommand(continueAfterDESCRIBE);

        env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
	});

    // 创建解码和渲染线程
    _decodeThread = std::thread([this]
    {
        _decoder = VideoDecoderFactory::CreateDecoder(DecoderType::FFmpeg);
        _render = RenderFactory::CreateRender(RenderType::D3D);

        while (_run)
        {
            auto readFrame = globalFrameBuffer.PopFrame();
            if (readFrame)
            {
                YUVFrame yuvFrame;
                _decoder->Decode(readFrame->GetDataPointer(), readFrame->GetDataSize(), yuvFrame);
                _render->Render(yuvFrame);
            }
        }

	});
    
    return true;
}

void RTSPClientWrapper::Start()
{
}

void RTSPClientWrapper::Stop()
{
}
