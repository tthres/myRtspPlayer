#pragma once

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "StreamClientState.h"
#include "DummySink.h"

void subsessionAfterPlaying(void* clientData);
void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
void setupNextSubsession(RTSPClient* rtspClient);

class ourRTSPClient: public RTSPClient {
public:
  static ourRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL,
				  int verbosityLevel = 0,
				  char const* applicationName = NULL,
				  portNumBits tunnelOverHTTPPortNum = 0);

protected:
  ourRTSPClient(UsageEnvironment& env, 
            char const* rtspURL,
		    int verbosityLevel, 
            char const* applicationName, 
            portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();

  virtual ~ourRTSPClient();

public:
  StreamClientState scs;
};