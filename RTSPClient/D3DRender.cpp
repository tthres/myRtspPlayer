#include "D3DRender.h"

#include <iostream>
#include <d3d9.h>
#include <Windows.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

D3DRender::D3DRender()
    : m_pDirect3D9(NULL)
    , m_pDirect3DDevice(NULL)
    , m_pDirect3DSurfaceRender(NULL)
    , m_bIsYuv(false)
    , m_nWidth(0)
    , m_nHeight(0)
    , yourHwnd(NULL)
{
    m_nWidth = 160; // YUV帧的宽度
    m_nHeight = 128; // YUV帧的高度

    const TCHAR szClassName[] = TEXT("MyAppClass");
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = szClassName;
    RegisterClass(&wc);

    yourHwnd = CreateWindowEx(
        0,                  // Extended style
        szClassName,        // Class name
        TEXT("My Window"),  // Window title
        WS_OVERLAPPEDWINDOW,// Window style
        CW_USEDEFAULT, CW_USEDEFAULT, m_nWidth, m_nHeight,  // x, y, width, height
        NULL,               // Parent window
        NULL,               // Menu
        GetModuleHandle(NULL),   // Instance handle
        NULL                // Additional application data
    );

    if (yourHwnd == NULL) {
        std::cerr << "Failed to create window." << std::endl;
    }

    ShowWindow(yourHwnd, SW_SHOW);
    UpdateWindow(yourHwnd);

    Init(yourHwnd, m_nWidth, m_nHeight, true);
}

D3DRender::~D3DRender()
{
}

int D3DRender::Init(HWND hwnd, unsigned int nWidth, unsigned int nHeight, bool isYuv)
{
    HRESULT lRet;
    InitializeCriticalSection(&m_critial);
    Cleanup();

    m_pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_pDirect3D9 == NULL)
        return -1;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    GetClientRect(hwnd, &m_rtViewport);

    lRet = m_pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDirect3DDevice);
    if (FAILED(lRet))
        return -1;

    if (isYuv) {
        lRet = m_pDirect3DDevice->CreateOffscreenPlainSurface(nWidth, nHeight, (D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2'), D3DPOOL_DEFAULT, &m_pDirect3DSurfaceRender, NULL);
        if (FAILED(lRet))
            return -1;
    }
    else {
        lRet = m_pDirect3DDevice->CreateOffscreenPlainSurface(nWidth, nHeight, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_pDirect3DSurfaceRender, NULL);
        if (FAILED(lRet))
            return -1;
    }

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_bIsYuv = isYuv;

    return 0;
}

void D3DRender::Cleanup()
{
    EnterCriticalSection(&m_critial);
    if (m_pDirect3DSurfaceRender)
        m_pDirect3DSurfaceRender->Release();
    if (m_pDirect3DDevice)
        m_pDirect3DDevice->Release();
    if (m_pDirect3D9)
        m_pDirect3D9->Release();
    LeaveCriticalSection(&m_critial);

}

bool D3DRender::Render(const YUVFrame& frame)
{
    if (!m_pDirect3DSurfaceRender)
        return false;

    HRESULT lRet;
    D3DLOCKED_RECT d3d_rect;
    // LockRect()锁定纹理上的一块矩形区域，该矩形区域被映射成像素数组。利用函数返回的D3DLOCKED_RECT结构体，可以对数组中的像素进行直接存取。
    lRet = m_pDirect3DSurfaceRender->LockRect(&d3d_rect, NULL, D3DLOCK_DONOTWAIT);
    if (FAILED(lRet))
        return false;

    int yStride = d3d_rect.Pitch;            // Pitch：surface中一行像素的数据量（Bytes）
    int uvStride = yStride / 2;

    byte* yDest = (BYTE*)d3d_rect.pBits;	// pBits：指向被锁定的数据。
    auto* uDest = yDest + yStride * m_nHeight;
    auto* vDest = uDest + uvStride * m_nHeight / 2;
   

    unsigned long i = 0;
    if (m_bIsYuv) {  //如果送入的数据是YUV420P，需要注意它与YV12的区别（U,V的位置刚好相反）
        for (i = 0; i < m_nHeight; i++) {
            memcpy(yDest + i * yStride, frame.y.y + i * frame.y.linesize, m_nWidth);
        }
        for (i = 0; i < m_nHeight / 2; i++) {
            memcpy(vDest + i * uvStride, frame.u.u + i * frame.u.linesize, m_nWidth / 2);
        }
        for (i = 0; i < m_nHeight / 2; i++) {
            memcpy(uDest + i * uvStride, frame.v.v + i * frame.v.linesize, m_nWidth / 2);
        }
    }
    else {
      /*  int pixel_w_size = m_nWidth * 4;
        for (i = 0; i < m_nHeight; i++) {
            memcpy(pDest, pSrc, pixel_w_size);
            pDest += stride;
            pSrc += pixel_w_size;
        }*/
    }

    lRet = m_pDirect3DSurfaceRender->UnlockRect();
    if (FAILED(lRet))
        return false;

    if (m_pDirect3DDevice == NULL)
        return false;

    m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    m_pDirect3DDevice->BeginScene();
    IDirect3DSurface9* pBackBuffer = NULL;

    m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
    m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rtViewport, D3DTEXF_LINEAR);
    m_pDirect3DDevice->EndScene();
    m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);     // 显示后备缓冲区的内容
    pBackBuffer->Release();

    return true;
    
}
