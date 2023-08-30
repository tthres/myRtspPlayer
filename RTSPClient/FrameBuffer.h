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
	std::deque<std::shared_ptr<FrameSample>> _tempFrameBuffer;	// ��Ž��յ���֡
	std::deque<std::shared_ptr<FrameSample>> _readyBuffer;		// ��Ű���Ƶ״̬�߼�������֡

	size_t _maxDelayCapacity = 30;								// �ӳ���󻺳壬�ﵽʱ��������л���
	FrameTypeState _currentState = NONE_STATE;
	int _frameCount = 0;
	int _frameIndex = 0;
	int _h264SliceCount = 0;
};
