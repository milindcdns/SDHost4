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
 * sdio_debug.h
 * SD Host controller driver debug module
 *****************************************************************************/


#ifndef SDIO_DEBUG_H
#define SDIO_DEBUG_H

#include "sdio_config.h"
#include <cdn_stdint.h>

#ifdef DEBUG
    #define DEBUG_DUMP_BUFFER32(b, bs, d) DebugDumpBuffer32((b), (bs), (d))
/*****************************************************************************/
/*!
 * @fn      void DebugDumpBuffer32(const uint32_t *bufferPtr, uint8_t bufferSize,
 *                                 const char* description)
 * @brief   Function dumps buffer
 * @param	bufferPtr - pointer to buffer with data to dump
 * @param	bufferSize - size of buffer in bytes it should be aligned to 4
 * @param	description - header of buffer dumped buffer
 *                  it is printed before buffer content
 * @param   Format Charaters of format
 */
/*****************************************************************************/
void DebugDumpBuffer32(const uint32_t *bufferPtr, uint32_t bufferSize,
                       const char* description);

#else
    #define DEBUG_DUMP_BUFFER32(b, bs, d)
#endif

#endif
