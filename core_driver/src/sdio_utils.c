/******************************************************************************
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
 * sdio_utils.c
 * Auxiliary driver functions
 *****************************************************************************/

#include "sdio_utils.h"
#include "sdio_errors.h"
#include "sdio_host.h"

/*****************************************************************************/
void DataSet(void *Destination, uint8_t Value, size_t SizeArg)
{
    size_t Size = SizeArg;
    size_t i;

    if ((Size & 3U) != 0U) {
        uint8_t *Dst = Destination;

        for (i = 0; i < Size; i++) {
            Dst[i] = Value;
        }
    }
    else {
        uint32_t *Dst = Destination;
        uint32_t val32 = ((uint32_t)Value << 24) | ((uint32_t)Value << 16)
                         | ((uint32_t)Value << 8) | Value;

        Size = Size >> 2;

        for (i = 0; i < Size; i++) {
            Dst[i] = val32;
        }
    }
}
/*****************************************************************************/

/******************************************************************************/
uint8_t WaitForValue(volatile uint32_t* Address, uint32_t Mask, uint8_t IsSet,
                     uint32_t TimeUs)
{
    uint8_t status = SDIO_ERR_NO_ERROR;
    uint32_t TimeNs = TimeUs * 1000 + 100;

    if (IsSet == 0U) {
        // wait until bit/bits will clear
        while (((CPS_REG_READ(Address) & Mask) != 0U)) {
            CPS_DelayNs(100);
            if (TimeNs < 100) {
                status = SDIO_ERR_TIMEOUT;
                break;
            }
            TimeNs -= 100;

        }
    } else {
        // wait until a bit/bits will set
        while (((CPS_REG_READ(Address) & Mask) == 0U)) {
            CPS_DelayNs(100);
            if (TimeNs < 100) {
                status = SDIO_ERR_TIMEOUT;
                break;
            }
            TimeNs -= 100;
        }
    }
    return (status);
}
/******************************************************************************/


static uint32_t Timer[SDIO_SLOT_COUNT];
/******************************************************************************/
void RetuningSetTimer(uint32_t Seconds, uint8_t TimerNumber)
{
    Timer[TimerNumber] = Seconds;
}
/******************************************************************************/

/******************************************************************************/
void RetuningGetTimer(uint32_t *Seconds, uint8_t TimerNumber)
{
    if (Timer[TimerNumber] > 0U) {
        Timer[TimerNumber]--;
    }
    *Seconds = Timer[TimerNumber];
}
/******************************************************************************/

uint32_t swap32(uint32_t data)
{
    return ((data & 0xFF000000U) >> 24) |
           ((data & 0x00FF0000U) >> 8) |
           ((data & 0x0000FF00U) << 8) |
           ((data & 0x000000FFU) << 24);
}

uint16_t swap16(uint16_t data)
{
    return ((data & 0xFF00U) >> 8)  |
           ((data & 0x00FFU) << 8);
}

