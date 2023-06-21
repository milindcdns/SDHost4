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

 *
 ******************************************************************************
 * sdio_utils.h
 * Auxiliary driver functions
 *****************************************************************************/

#ifndef SDIO_UTILS
#define SDIO_UTILS

#include "cdn_stdint.h"
#include <stdio.h>

/// macro gets one byte from dword
#define GetByte(dword, byte_nr)     (((dword) >> ((byte_nr) * 8U)) & 0xFFU)

/*****************************************************************************/
/*!
 * @fn          void DataSet(void *Destination, uint8_t Value, uint32_t Size)
 * @brief       Function copies data from source do destination
 * @param       Destination bufer for set
 * @param       Value data to set
 * @param       Size size in bytes of Destionation buffer
 * @return      Function returns 0 if everything is ok
 *                  otherwise returns error number
 */
/*****************************************************************************/
void DataSet(void *Destination, uint8_t Value, size_t SizeArg);

/*****************************************************************************/
/*!
 * @fn          uint8_t WaitForValue(uint32_t* Address, uint32_t Mask, uint8_t IsSet,
 *                                  uint32_t Time)
 * @brief       Function waits until data at address Address masked with Mask
 *                  will be equal 1 or 0 (depend on IsSet parameter)
 * @param       Address Address of data which is checking
 * @param       Mask This parameter defines which bits
 *                  should be checked and masked
 * @param       IsSet This parameter defines if selected by mask bits should
 *                  be set or cleared
 * @param       Time it defines in microseconds after checking will be finished
 *                  with error
 * @return      Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t WaitForValue(volatile uint32_t* Address, uint32_t Mask, uint8_t IsSet, uint32_t Time);


/*****************************************************************************/
/*!
 * @fn          void RetuningSetTimer(uint32_t Seconds, uint8_t TimerNumber)
 * @brief       Function sets Timer with Seconds parameter value.
 *                  Timer is used to specify when re-tuning procedure
 *                  have to be executed
 * @param       Seconds number of seconds to set
 * @param       TimerNumber timer number to set
 * @return      Function returns 0 if everything is ok
 *                  otherwise returns error number
 */
/*****************************************************************************/
void RetuningSetTimer(uint32_t Seconds, uint8_t TimerNumber);

/*****************************************************************************/
/*!
 * @fn          void RetuningGetTimer(uint32_t *Seconds, uint8_t TimerNumber)
 * @brief       Function gets Timer value.
 *                  Timer is used to specify when re-tuning procedure
 *                  have to be executed
 * @param       Seconds number of seconds to set
 * @param       TimerNumber timer number to set
 * @return      Function returns 0 if everything is ok
 *                  otherwise returns error number
 */
/*****************************************************************************/
void RetuningGetTimer(uint32_t *Seconds, uint8_t TimerNumber);

uint32_t GetTimeUs(void);
uint8_t IsTimeAfter(uint32_t startTime, uint32_t requestedTime);

#define GetMax(a, b)    (((a) > (b)) ? (a) : (b))
#define GetMin(a, b)    (((a) < (b)) ? (a) : (b))
#define CpuToLe32(a)		    (a)
#define LeToCpu32(a)            (a)

uint32_t swap32(uint32_t data);
uint16_t swap16(uint16_t data);

#endif
