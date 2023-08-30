#include "FrameBuffer.h"
#include "RTSPClientWrapper.h"


#include <iostream>



FrameBuffer::FrameBuffer()
{
}

void FrameBuffer::PushFrame(std::shared_ptr<FrameSample> frame)
{
	std::lock_guard<std::mutex> locker(_mtx);

	/*if (_needKeyFrame)
	{
		if (!frame->IsKeyFrame())
		{
			std::cout << "FrameBuffer::PushFrame: need key frame, but not key frame, drop it." << std::endl;
			return;
		}
		if (frame->IsCorrupted())
		{
			std::cout << "FrameBuffer::PushFrame: need key frame, but corrupted, drop it." << std::endl;
			return;
		}
		_needKeyFrame = false;
	}
	else 
	{
		if (frame->IsCorrupted())
		{
			if (frame->IsKeyFrame())
			{
				std::cout << "drop corruted keyframe" << std::endl;
			}
			else
			{
				std::cout << "drop corruted frame" << std::endl;
			}
			_needKeyFrame = true;
			return;
		}
	}

	BufferState state = USABLE;
	
	if (frame->GetFrameType() == FrameSample::FrameType::H264)
	{
		if (_h264SliceCount == 0)
		{
			UpdateH264SliceCount(frame);
		}
		state = UpdateH264CurrentState(frame->GetFrameTypeState());
	}
	else
	{
		std::cout << " not support frame type : " << frame->GetFrameType() << std::endl;
		return;
	}*/

	_tempFrameBuffer.push_back(frame);
	/*if (!DealStateAndUpdateBuffer(state))
	{
		_needKeyFrame = true;
	}*/
	ClearReadyBufferWhileFull();
}

std::shared_ptr<FrameSample> FrameBuffer::PopFrame()
{
	std::lock_guard<std::mutex> locker(_mtx);
	std::shared_ptr<FrameSample> frame = nullptr;
	if (!_tempFrameBuffer.empty())
	{
		frame = _tempFrameBuffer.front();
		_tempFrameBuffer.pop_front();
	}
	return frame;
}

int FrameBuffer::Size()
{
	return _readyBuffer.size();
}

std::shared_ptr<FrameSample> FrameBuffer::CreateFrameSample(size_t size, FrameSample::FrameType type)
{
	return std::shared_ptr<FrameSample>();
}

bool FrameBuffer::DealStateAndUpdateBuffer(BufferState state)
{
	bool result(true);
	if (state == USABLE)
	{
		std::cout << "display buffer state is usable" << std::endl;
		auto mergedFrame = MergeFrame(_tempFrameBuffer);
		if (mergedFrame)
		{
			_readyBuffer.push_back(mergedFrame);
		}

		_tempFrameBuffer.clear();
	}
	else if (state == FATAL)
	{
		std::cout << "display buffer state is not true" << std::endl;
		_tempFrameBuffer.clear();
	}
	else if (state == NEW_BEGIN)
	{
		std::cout << "display buffer state is at new begin" << std::endl;
		auto frame = _tempFrameBuffer.back();
		_tempFrameBuffer.clear();
		if (frame)
			_tempFrameBuffer.push_back(frame); // store vps/hevc or sps/h264 frame
	}

	return result;
}

void FrameBuffer::ClearReadyBufferWhileFull()
{
	if (_readyBuffer.size() >= _maxDelayCapacity)
	{
		std::cout << "reach max delay capacity, frame buffer size is " << _readyBuffer.size() << std::endl;
		auto reserveIter = _readyBuffer.begin();
		// reserve relative frame used by decoder
		if (*reserveIter)
		{
			const auto timeStamp = (*reserveIter)->GetTimestamp();
			++reserveIter;
			while (reserveIter != _readyBuffer.end())
			{ // strict check, basically it won't be occurs if _maxDelayCapacity greater than HEVC_SLICE_COUNT
				if ((*reserveIter)->GetTimestamp() != timeStamp)
					break;
				++reserveIter;
			}
		}
		else
		{
			return;
		}

		auto toDeleteBeginIter = reserveIter;
		auto toDeleteEndIter = reserveIter;
		auto foundKeyFrameIter = toDeleteEndIter;
		while (foundKeyFrameIter != _readyBuffer.end())
		{
			if ((*foundKeyFrameIter)->IsKeyFrame())
			{
				toDeleteEndIter = foundKeyFrameIter;
			}
			++foundKeyFrameIter;
		}
		_readyBuffer.erase(toDeleteBeginIter, toDeleteEndIter); // safe to delete the same iter, which do nothing
		std::cout << "after frame buffer size is " << _readyBuffer.size() << std::endl;
	}
}

void FrameBuffer::UpdateH264SliceCount(std::shared_ptr<FrameSample> sample)
{
	/*if (sample->GetFrameType() == FrameSample::H264 && sample->GetFrameTypeState() == PPS_FRAME_STATE)
	{
		auto data = sample->GetDataPointer();
		const int startCodeIndex = GetStartCodeIndex(data);
		const int count = GetH264SliceCount(data + startCodeIndex, sample->GetDataSize() - startCodeIndex);
		if (count > 0)
		{
			_h264SliceCount = count;
		}
	}*/
}

FrameBuffer::BufferState FrameBuffer::UpdateH264CurrentState(FrameTypeState state)
{
	std::cout << "display buffer state is : " << _currentState << "state is : " << state << ", frame count is : " << _frameCount << std::endl;
	BufferState bufferState = UNUSABLE;
	switch (_currentState)
	{
	case NONE_STATE:
		if (state != SPS_FRAME_STATE)
		{
			bufferState = FATAL;
		}
		else
		{
			_frameCount = 0;
			_currentState = SPS_FRAME_STATE;
		}
		break;
	case SPS_FRAME_STATE:
		if (state != PPS_FRAME_STATE)
		{
			bufferState = FATAL;
		}
		else
		{
			_currentState = PPS_FRAME_STATE;
		}
		break;
	case PPS_FRAME_STATE:
		if (state != IDR_FRAME_STATE)
		{
			bufferState = FATAL;
		}
		else
		{
			_currentState = IDR_FRAME_STATE;
			++_frameCount;
			if (_frameCount >= _h264SliceCount)
			{
				_frameCount = 0;
				bufferState = USABLE;
			}
		}
		break;
	case IDR_FRAME_STATE:
		_currentState = state;
		if (state == IDR_FRAME_STATE)
			++_frameCount;
		if (_frameCount >= _h264SliceCount)
		{
			_frameCount = 0;
			bufferState = USABLE;
		}
		if (state != IDR_FRAME_STATE)
		{
			if (state == P_FRAME_STATE)
			{
				if (_frameCount != 0)
				{// lost idr frame
					bufferState = FATAL;
				}
				else
				{// idr end, p frame begin
					++_frameCount;
					bufferState = UNUSABLE;
					if (_frameCount >= _h264SliceCount)
					{
						_frameCount = 0;
						bufferState = USABLE;
					}
				}
			}
			else
			{
				bufferState = FATAL;
			}
		}
		break;
	case P_FRAME_STATE:
		_currentState = state;
		if (state == P_FRAME_STATE)
			++_frameCount;
		if (_frameCount >= _h264SliceCount)
		{
			bufferState = USABLE;
			_frameCount = 0;
		}
		if (state != P_FRAME_STATE)
		{
			bufferState = FATAL;
		}
		break;
	default:
		_currentState = state;
		break;
	}

	if (bufferState == FATAL)
	{
		_currentState = NONE_STATE;
	}

	if (state == SPS_FRAME_STATE)
	{
		_currentState = SPS_FRAME_STATE;
		bufferState = NEW_BEGIN;
	}

	return bufferState;
}

std::shared_ptr<FrameSample> FrameBuffer::MergeFrame(std::deque<std::shared_ptr<FrameSample>> tempSamples)
{
	return std::shared_ptr<FrameSample>();
}
