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

#ifndef _BASESTREAMCONVERTER_H
#define	_BASESTREAMCONVERTER_H

#include "common.h"
#include "streaming/streamcapabilities.h"

class BaseStreamConverter {
protected:
	uint64_t _fromType;
	uint64_t _toType;
	IOBuffer _output;
	SparseBuffer _sparseBuffer;
	StreamCapabilities *_pCapabilities;
public:
	BaseStreamConverter(uint64_t fromType, uint64_t toType,
			StreamCapabilities *pCapabilities);
	virtual ~BaseStreamConverter();

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio) = 0;

	virtual bool FeedData(SparseBuffer &buffers, uint32_t processedLength,
			uint32_t totalLength, double absoluteTimestamp, bool isAudio) = 0;

	virtual void ComputeHeaders(bool isAudio) = 0;

	IOBuffer &GetBuffer();
};

#endif	/* _BASESTREAMCONVERTER_H */

