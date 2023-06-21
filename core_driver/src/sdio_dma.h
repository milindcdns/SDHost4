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
 * sdio_dma.h
 * SD Host controller driver DMA module
 *****************************************************************************/

#ifndef SDIO_DMA_H
#define SDIO_DMA_H

#include "sdio_types.h"

/*****************************************************************************/
/*!
 * @fn      uint8_t DMA_PrepareTransfer(CSDD_SDIO_Slot* pSlot,
 *                                  CSDD_Request* pRequest)
 * @brief       Function prepare DMA module to data transfer
 * @param       pSlot Slot on which the DMA transfer shall be execute
 * @param       pRequest it describes request to execute
 * @return      Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t DMA_PrepareTransfer(CSDD_SDIO_Slot* pSlot, CSDD_Request* pRequest);

/*****************************************************************************/
/*!
 * @fn      uint8_t DMA_PrepareTransfer(CSDD_SDIO_Slot* pSlot,
 *                                  CSDD_Request* pRequest)
 * @brief   Function handles DMA interrupt
 * @param   pSlot Slot on which DMA interrupt occured
 * @param   pRequest it describes request which
 *              is currently executed
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t DMA_HandleInterrupt(CSDD_SDIO_Slot* pSlot, CSDD_Request* pRequest, uint32_t Status);

/*****************************************************************************/
/*!
 * @fn      uint8_t DMA_FinishTransfer(CSDD_SDIO_Slot* pSlot,
 *                                  CSDD_Request* pRequest)
 * @brief   Function finishes DMA transfer
 * @param   pSlot Slot on which the DMA transfer is executing
 * @param   pRequest it describes executing request
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t DMA_FinishTransfer(CSDD_SDIO_Slot* pSlot, CSDD_Request* pRequest);

/*****************************************************************************/
/*!
 * @fn      uint8_t DMA_FinishTransfer(CSDD_SDIO_Slot* pSlot,
 *                                  CSDD_Request* pRequest)
 * @brief   Function selects optimal DMA transmission mode
 * @param   pSlot Slot on which the DMA transfer shall be execute
 * @param   pRequest it describes request to execute
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t DMA_SpecifyTransmissionMode(CSDD_SDIO_Slot* pSlot, CSDD_Request* pRequest);

#endif
