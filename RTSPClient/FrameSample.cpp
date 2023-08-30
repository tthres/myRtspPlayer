#include "FrameSample.h"

FrameSample::FrameSample(size_t size, FrameType type)
	: _innerBuffer(std::make_shared<std::vector<uint8_t>>(size))
	, _dataSize(size)
	, _frameType(type)
{
}

FrameSample::~FrameSample()
{
}

bool FrameSample::IsKeyFrame()
{
	return _isKeyFrame;
}

void FrameSample::SetKeyFrame(bool iskey)
{
	_isKeyFrame = iskey;
}

bool FrameSample::IsCorrupted()
{
	return _isCorrutped;
}

void FrameSample::SetIsCorrutped(bool isCorrutped)
{
	_isCorrutped = isCorrutped;
}

uint8_t* FrameSample::GetDataPointer()
{
	if (_innerBuffer)
	{
		return _innerBuffer->data();
	}
	return nullptr;
}

void FrameSample::SetFrameTypeState(FrameTypeState state)
{
	_frameTypeState = state;
}

FrameTypeState FrameSample::GetFrameTypeState() const
{
	return _frameTypeState;
}

void FrameSample::SetDataSize(uint32_t size)
{
	_dataSize = size;
	_innerBuffer->resize(size);
}

uint32_t FrameSample::GetDataSize()
{
	return _dataSize;
}

uint64_t FrameSample::GetTimestamp()
{
	return time_stamp_;
}
