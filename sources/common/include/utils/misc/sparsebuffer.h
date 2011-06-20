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

#ifndef _SPARSEBUFFER_H
#define	_SPARSEBUFFER_H

#include "platform/platform.h"

typedef struct _SparseBufferElement {
	uint8_t *pBuffer;
	uint32_t length;
} SparseBufferElement;

typedef struct _SparseBuffer {
	uint32_t count;
	SparseBufferElement *pBuffers;
	_SparseBuffer();
	virtual ~_SparseBuffer();
	void Init(uint32_t _count);
	void Free();
	operator string();
} SparseBuffer;

#define SET_SPARSE_BUFFER(sparseBuffer,index,buffer,length) \
do { \
	assert((sparseBuffer).count>(index)); \
	(sparseBuffer).pBuffers[(index)].pBuffer=(buffer); \
	(sparseBuffer).pBuffers[(index)].(length)=(length); \
} while(0)

#endif	/* _SPARSEBUFFER_H */

