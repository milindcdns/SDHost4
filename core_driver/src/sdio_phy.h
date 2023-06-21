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
 * sdio_phy.h
 * SD Host controller driver - PHY module
 *****************************************************************************/


#ifndef SDIO_PHY_H
#define SDIO_PHY_H

#include "sdio_types.h"

/*****************************************************************************/
/*!
 * @fn      uint8_t SDIO_Phy_PhySettings_SDv4(const CSDD_SDIO_Host* pSdioHost,
 *			    const CSDD_PhyDelaySettings *PhyDelaySet)
 * @brief   Function sets configures delay line in UHS-I PHY hardware module.
 *		Function works for SD Host 4
 * @param   pSdioHost pointer to sdio host object
 * @param   PhyDelaySet PHY delay settings
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t SDIO_Phy_PhySettings_SDv4(const CSDD_SDIO_Host*             pSdioHost,
                                  const CSDD_PhyDelaySettings *PhyDelaySet);

/*****************************************************************************/
/*!
 * @fn      uint8_t SDIO_Phy_PhySettings_SDv4(const CSDD_SDIO_Host* pSdioHost,
 *			    const CSDD_PhyDelaySettings *PhyDelaySet)
 * @brief   Function sets configures delay line in UHS-I PHY hardware module.
 *		Function works for SD Host 3
 * @param   pSlot pointer to sdio slot object
 * @param   PhyDelaySet PHY delay settings
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t SDIO_Phy_PhySettings_SDv3(const CSDD_SDIO_Slot*             pSlot,
                                  const CSDD_PhyDelaySettings *PhyDelaySet);

uint8_t SDIO_ReadPhySet(const CSDD_SDIO_Host* pSdioHost, uint8_t slotIndex,
                        CSDD_PhyDelay delayType, uint8_t *delayVal);

uint8_t SDIO_WritePhySet(const CSDD_SDIO_Host* pSdioHost, uint8_t slotIndex,
                         CSDD_PhyDelay delayType, uint8_t delayVal);

uint32_t SDIO_CPhy_DLLReset(CSDD_SDIO_Host* pSdioHost, bool doReset);

void SDIO_CPhy_SetCPhyConfigIoDelay(CSDD_SDIO_Host* pSdioHost, CSDD_CPhyConfigIoDelay* ioDelay);
void SDIO_CPhy_GetCPhyConfigIoDelay(CSDD_SDIO_Host* pSdioHost, CSDD_CPhyConfigIoDelay* ioDelay);
void SDIO_CPhy_SetConfigLvsi(CSDD_SDIO_Host* pSdioHost, CSDD_CPhyConfigLvsi* lvsi);
void SDIO_CPhy_GetConfigLvsi(CSDD_SDIO_Host* pSdioHost, CSDD_CPhyConfigLvsi* lvsi);
void SDIO_CPhy_SetConfigDfiRd(CSDD_SDIO_Host* pSdioHost, CSDD_CPhyConfigDfiRd* dfiRd);
void SDIO_CPhy_GetConfigDfiRd(CSDD_SDIO_Host* pSdioHost, CSDD_CPhyConfigDfiRd* dfiRd);
void SDIO_CPhy_SetConfigOutputDelay(CSDD_SDIO_Host* pSdioHost,
                                       CSDD_CPhyConfigOutputDelay* outputDelay);
void SDIO_CPhy_GetConfigOutputDelay(CSDD_SDIO_Host* pSdioHost,
                                       CSDD_CPhyConfigOutputDelay* outputDelay);
void SDIO_CPhy_SetExtMode(CSDD_SDIO_Host* pSdioHost, bool extendedWrMode,
                             bool extendedRdMode);
void SDIO_CPhy_GetExtMode(CSDD_SDIO_Host* pSdioHost, bool* extendedWrMode,
                             bool* extendedRdMode);
void SDIO_CPhy_SetSdclkAdj(CSDD_SDIO_Host* pSdioHost, uint8_t sdclkAdj);
void SDIO_CPhy_GetSdclkAdj(CSDD_SDIO_Host* pSdioHost, uint8_t* sdclkAdj);

#endif
