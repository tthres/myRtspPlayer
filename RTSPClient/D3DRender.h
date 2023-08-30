#pragma once

#include <d3d9.h>

#include "yuv.h"
#include "IRender.h"

class D3DRender : public IRender
{
public:

	D3DRender();
	virtual ~D3DRender();

    virtual int Init(HWND hwnd, unsigned int nWidth, unsigned int nHeight, bool isYuv);
    virtual bool Render(const YUVFrame& frame);
    virtual void Cleanup();


private:
    bool                    m_bIsYuv;
    int                     m_nWidth;
    int                     m_nHeight;
    RECT                    m_rtViewport;
    CRITICAL_SECTION        m_critial;
    IDirect3D9*             m_pDirect3D9;
    IDirect3DDevice9*       m_pDirect3DDevice;
    IDirect3DSurface9*      m_pDirect3DSurfaceRender;

    HWND yourHwnd;

};

