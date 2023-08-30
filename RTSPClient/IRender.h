#pragma once

#include <windows.h>
#include "yuv.h"

enum class RenderType {
    D3D,
    OpenGL,
    // ... other decoder types
};

class IRender {
public:
    virtual ~IRender() {}

    virtual int Init(HWND hwnd, unsigned int nWidth, unsigned int nHeight, bool isYuv) = 0;
    virtual bool Render(const YUVFrame& frame) = 0;
    virtual void Cleanup() = 0;
    
};

