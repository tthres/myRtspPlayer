#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

#include "ourRTSPClient.h"
#include "StreamClientState.h"
#include "DummySink.h"
#include "DummyVideoSink.h"
#include "RTSPClientWrapper.h"

FrameBuffer globalFrameBuffer;

int main(int argc, char **argv)
{
  RTSPClientWrapper rtspClientWrapper;
  

  if (argc < 2)
  {
    rtspClientWrapper.usage(argv[0]);
    return 1;
  }

  for (int i = 1; i <= argc - 1; ++i)
  {
    rtspClientWrapper.play(argv[0], argv[i]);
  }

  return 0;
}
