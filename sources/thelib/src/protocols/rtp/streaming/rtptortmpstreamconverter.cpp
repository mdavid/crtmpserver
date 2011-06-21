/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *  
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "protocols/rtp/streaming/rtptortmpstreamconverter.h"
#include "streaming/streamstypes.h"
#include "streaming/nalutypes.h"

RTPToRTMPStreamConverter::RTPToRTMPStreamConverter(StreamCapabilities *pCapabilities)
: BaseStreamConverter(ST_IN_NET_RTP, ST_OUT_NET_RTMP, pCapabilities) {
	_pSPSPPS = new uint8_t[1024];
	_SPSPPSLength = 0;
	_PPSStart = 0;
	_videoCodecSent = false;
	_audioCodecSent = false;

	_pSPSPPS[0] = 0x17; //0x10 - key frame; 0x07 - H264_CODEC_ID
	_pSPSPPS[1] = 0; //0: AVC sequence header; 1: AVC NALU; 2: AVC end of sequence
	_pSPSPPS[2] = 0; //CompositionTime
	_pSPSPPS[3] = 0; //CompositionTime
	_pSPSPPS[4] = 0; //CompositionTime
	_pSPSPPS[5] = 1; //version
	_pSPSPPS[9] = 0xff; //6 bits reserved (111111) + 2 bits nal size length - 1 (11)
	_pSPSPPS[10] = 0xe1; //3 bits reserved (111) + 5 bits number of sps (00001)
	
	_audioCodec[0] = 0xaf;
	_audioCodec[1] = 0x01;
}

RTPToRTMPStreamConverter::~RTPToRTMPStreamConverter() {
	delete[] _pSPSPPS;
}

bool RTPToRTMPStreamConverter::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	if (isAudio)
		return FeedAudioData(pData, dataLength, absoluteTimestamp);
	else
		return FeedVideoData(pData, dataLength, absoluteTimestamp);
}

bool RTPToRTMPStreamConverter::FeedData(SparseBuffer &buffers, uint32_t processedLength,
		uint32_t totalLength, double absoluteTimestamp, bool isAudio) {
	ASSERT("operation not supported");
	return false;
}

void RTPToRTMPStreamConverter::ComputeHeaders(bool isAudio) {
	if (isAudio) {
		_output.IgnoreAll();
		if ((_pCapabilities == NULL)
				|| (_pCapabilities->audioCodecId != CODEC_AUDIO_AAC)) {
			return;
		}
		_output.ReadFromRepeat(0xaf, 1);
		_output.ReadFromRepeat(0x00, 1);
		_output.ReadFromBuffer(_pCapabilities->aac._pAAC,
				_pCapabilities->aac._aacLength);
	} else {
		_output.IgnoreAll();
		if ((_pCapabilities == NULL)
				|| (_pCapabilities->videoCodecId != CODEC_VIDEO_AVC)) {
			return;
		}

		//1. Prepare the SPS part from video codec
		memcpy(_pSPSPPS + 6, _pCapabilities->avc._pSPS + 1, 3); //profile,profile compat,level
		EHTONSP(_pSPSPPS + 11, (uint16_t) _pCapabilities->avc._spsLength);
		memcpy(_pSPSPPS + 13, _pCapabilities->avc._pSPS, _pCapabilities->avc._spsLength);
		_PPSStart = 13 + _pCapabilities->avc._spsLength;


		_pSPSPPS[_PPSStart] = 1;
		EHTONSP(_pSPSPPS + _PPSStart + 1, (uint16_t) _pCapabilities->avc._ppsLength);
		memcpy(_pSPSPPS + _PPSStart + 1 + 2, _pCapabilities->avc._pPPS, _pCapabilities->avc._ppsLength);

		_output.ReadFromBuffer(_pSPSPPS,
				_PPSStart + 1 + 2 + _pCapabilities->avc._ppsLength);
	}
}

bool RTPToRTMPStreamConverter::FeedAudioData(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp) {
	_output.IgnoreAll();
	_output.ReadFromBuffer(_audioCodec,2);
	_output.ReadFromBuffer(pData, dataLength);
	return true;
}

bool RTPToRTMPStreamConverter::FeedVideoData(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp) {
	_output.IgnoreAll();
	switch (NALU_TYPE(pData[0])) {
		case NALU_TYPE_SPS:
		case NALU_TYPE_PPS:
		{
			return true;
		}
		case NALU_TYPE_IDR:
		case NALU_TYPE_SLICE:
		{
			//10. Make room for the RTMP header
			_output.ReadFromRepeat(0, 9);

			//11. Add the raw data
			_output.ReadFromBuffer(pData, dataLength);

			uint8_t *pBuffer = GETIBPOINTER(_output);

			//12. Setup the RTMP header
			pBuffer[0] = (NALU_TYPE(pData[0]) == NALU_TYPE_IDR) ? 0x17 : 0x27;
			pBuffer[1] = 0x01;
			pBuffer[2] = pBuffer[3] = pBuffer[4] = 0;
			EHTONLP(pBuffer + 5, dataLength);

			return true;
		}
		case NALU_TYPE_PD:
		case NALU_TYPE_SEI:
		case NALU_TYPE_FILL:
		{
			return true;
		}
		default:
		{
			WARN("Ignoring NAL: %s", STR(NALUToString(pData[0])));
			return true;
		}
	}
}
