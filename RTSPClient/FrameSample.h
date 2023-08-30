#pragma once
#pragma once
#include <cstdint>
#include <memory>
#include <vector>

enum FrameTypeState
{
    NONE_STATE,
    VPS_FRAME_STATE,
    SPS_FRAME_STATE,
    PPS_FRAME_STATE,
    IDR_FRAME_STATE,
    P_FRAME_STATE,
};

class FrameSample
{
public:
    enum FrameType
    {
        UNKNOWN = -1,
        H264,
        HEVC
    };

	FrameSample(size_t size, FrameType type = UNKNOWN);
	~FrameSample();

    FrameSample(const FrameSample& other) = delete;
    FrameSample& operator=(const FrameSample& other) = delete;

    bool IsKeyFrame();
    void SetKeyFrame(bool iskey);

    bool IsCorrupted();
    void SetIsCorrutped(bool isCorrutped);

    uint8_t* GetDataPointer();

    void SetFrameTypeState(FrameTypeState state);
    FrameTypeState GetFrameTypeState() const;

    FrameType GetFrameType() const { return _frameType; }

    void SetDataSize(uint32_t size);
    uint32_t GetDataSize();

    uint64_t GetTimestamp();

private:
    std::shared_ptr<std::vector<uint8_t>> _innerBuffer;
    FrameTypeState _frameTypeState = NONE_STATE;
    FrameType _frameType;
    uint32_t _dataSize;
    uint64_t time_stamp_;
    bool _isKeyFrame = false;
    bool _isCorrutped = false;

};

