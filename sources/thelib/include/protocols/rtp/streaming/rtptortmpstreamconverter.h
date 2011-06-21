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

#ifndef _RTPTORTMPSTREAMCONVERTER_H
#define	_RTPTORTMPSTREAMCONVERTER_H

#include "streaming/basestreamconverter.h"

class RTPToRTMPStreamConverter
: public BaseStreamConverter {
private:
	uint8_t *_pSPSPPS;
	uint8_t _SPSPPSLength;
	uint32_t _PPSStart;
	bool _videoCodecSent;
	bool _audioCodecSent;
	uint8_t _audioCodec[2];
public:
	RTPToRTMPStreamConverter(StreamCapabilities *pCapabilities);
	virtual ~RTPToRTMPStreamConverter();

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);

	virtual bool FeedData(SparseBuffer &buffers, uint32_t processedLength,
			uint32_t totalLength, double absoluteTimestamp, bool isAudio);
	virtual void ComputeHeaders(bool isAudio);
private:
	bool FeedAudioData(uint8_t *pData, uint32_t dataLength, double absoluteTimestamp);
	bool FeedVideoData(uint8_t *pData, uint32_t dataLength, double absoluteTimestamp);
};


#endif	/* _RTPTORTMPSTREAMCONVERTER_H */

