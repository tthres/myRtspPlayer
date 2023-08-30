#pragma once
#include <cstdint>

struct YFrame
{
    const uint8_t* y;
    int32_t linesize;
};

struct UFrame
{
    const uint8_t* u;
    int32_t linesize;
};

struct VFrame
{
    const uint8_t* v;
    int32_t linesize;
};

struct YUVFrame
{
    int32_t width;
    int32_t height;

    YFrame y;
    UFrame u;
    VFrame v;
};