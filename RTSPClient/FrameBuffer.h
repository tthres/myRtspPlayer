#pragma once

#include "FrameSample.h"
#include <deque>
#include <mutex>

class FrameBuffer
{
public:
	enum BufferState
	{
		FATAL,
		USABLE,
		UNUSABLE,
		NEW_BEGIN
	};

	FrameBuffer();
	void PushFrame(std::shared_ptr<FrameSample> frame);
	std::shared_ptr<FrameSample> PopFrame();

	int Size();

	std::shared_ptr<FrameSample> CreateFrameSample(size_t size, FrameSample::FrameType type = FrameSample::FrameType::UNKNOWN);
	bool DealStateAndUpdateBuffer(BufferState state);
	void ClearReadyBufferWhileFull();

	void UpdateH264SliceCount(std::shared_ptr<FrameSample> sample);
	BufferState UpdateH264CurrentState(FrameTypeState state);
	std::shared_ptr<FrameSample> MergeFrame(std::deque<std::shared_ptr<FrameSample>> tempSamples);

private:
	std::mutex _mtx;
	bool _needKeyFrame = true;
	std::deque<std::shared_ptr<FrameSample>> _tempFrameBuffer;	// 存放接收到的帧
	std::deque<std::shared_ptr<FrameSample>> _readyBuffer;		// 存放按视频状态逻辑处理后的帧

	size_t _maxDelayCapacity = 30;								// 延迟最大缓冲，达到时将清空所有缓冲
	FrameTypeState _currentState = NONE_STATE;
	int _frameCount = 0;
	int _frameIndex = 0;
	int _h264SliceCount = 0;
};
