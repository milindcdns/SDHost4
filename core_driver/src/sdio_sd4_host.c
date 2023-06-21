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
 * sdio_sd4_host.c
 * Vendor specific registers definitions for SD Host 4 controller
 *****************************************************************************/

#include "sdio_host.h"
#include "csdd_structs_if.h"

#ifndef SDIO_CFG_HOST_VER
    #error "SDIO_CFG_HOST_VER should be defined explicitly"
#endif

#if SDIO_CFG_HOST_VER >= 4

#include "sdio_sd4_host.h"

void SDIOHost_AxiErrorInit(CSDD_SDIO_Host *pSdioHost)
{
    uint32_t reg;
    reg = CPS_REG_READ(&pSdioHost->RegOffset->HRS.HRS03);
    reg |= HRS3_SET_INT_STATUS_EN(HRS3_AER_ALL);
    CPS_REG_WRITE(&pSdioHost->RegOffset->HRS.HRS03, reg);
}

void SDIOHost_AxiErrorIntSignalCfg(CSDD_SDIO_Host *pSdioHost, uint8_t enable)
{
    uint32_t reg;
    reg = CPS_REG_READ(&pSdioHost->RegOffset->HRS.HRS03);
    if (enable != 0U) {
        reg |= HRS3_SET_INT_SIGNAL_EN(HRS3_AER_ALL);
    } else {
        reg &= ~HRS3_SET_INT_SIGNAL_EN(HRS3_AER_ALL);
    }
    CPS_REG_WRITE(&pSdioHost->RegOffset->HRS.HRS03, reg);
}

void SDIOHost_AxiErrorGetStatus(CSDD_SDIO_Host *pSdioHost, uint8_t* status)
{
    uint32_t reg;
    reg = CPS_REG_READ(&pSdioHost->RegOffset->HRS.HRS03);
    *status = (uint8_t)HRS3_GET_INT_STATUS(reg);
}

void SDIOHost_AxiErrorClearStatus(CSDD_SDIO_Host *pSdioHost, uint8_t status)
{
    uint32_t reg;
    reg = CPS_REG_READ(&pSdioHost->RegOffset->HRS.HRS03);
    reg &= ~HRS3_SET_INT_STATUS(HRS3_AER_ALL);
    reg |= HRS3_SET_INT_STATUS(status);
    CPS_REG_WRITE(&pSdioHost->RegOffset->HRS.HRS03, reg);
}

#endif
