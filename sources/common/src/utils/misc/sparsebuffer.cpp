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

#include "utils/misc/sparsebuffer.h"
#include "utils/buffering/iobuffer.h"

_SparseBuffer::_SparseBuffer() {
	count = 0;
	pBuffers = NULL;
}

_SparseBuffer::~_SparseBuffer() {
	Free();
}

void _SparseBuffer::Init(uint32_t _count) {
	Free();
	pBuffers = new SparseBufferElement[_count];
	memset(pBuffers, 0, sizeof (SparseBufferElement) * _count);
	count = _count;
}

void _SparseBuffer::Free() {
	if (pBuffers != NULL) {
		delete[] pBuffers;
		pBuffers = NULL;
	}
	count = 0;
}

_SparseBuffer::operator string() {
	string result = format("buffers count: %u\n", count);
	for (uint32_t i = 0; i < count; i++) {
		result += format("i: %u\n", i);
		if (pBuffers[i].length != 0) {
			result += IOBuffer::DumpBuffer(
					pBuffers[i].pBuffer, pBuffers[i].length);
		} else {
			result += "NULL";
		}
		if (i != (count - 1))
			result += "\n";
	}
	return result;
}
