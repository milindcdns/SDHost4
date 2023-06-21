/******************************************************************************
*
* (C) 2023 Cadence Design Systems, Inc. 
*
******************************************************************************
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*
 * Common file for test
 *****************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <cdn_stdint.h>

#define CHECK_STATUS(status)\
    if (status){\
        printf("%s:%d - error 0x%x \n", __FILE__, __LINE__, status);\
	return status;\
    }

#define SubPrint printf

extern unsigned char *writeBuffer;
extern unsigned char *readBuffer;

void Fillbuf(void *buffer, uint32_t size, uint32_t start);
void Clearbuf(void *buffer, uint32_t size,uint32_t dataPattern);
uint32_t Comparebuf(void *b1, void *b2, uint32_t size);
uint8_t PrintBuffers(int size);
void PrintBuffers2(void *buffer1, void *buffer2, uint32_t size);


#endif
