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

 **********************************************************************
 * WARNING: This file is auto-generated using api-generator utility.
 *          api-generator: 13.05.b3ee589
 *          Do not edit it manually.
 **********************************************************************
 * Cadence Combo PHY Core Driver
 **********************************************************************/

#include "ccp_obj_if.h"

/* parasoft suppress item METRICS-41-3 "Number of blocks of comments per statement, DRV-4926" */

CCP_OBJ *CCP_GetInstance(void)
{
    static CCP_OBJ driver =
    {
        .probe = CCP_Probe,
        .init = CCP_Init,
        .setPhyDqTimingReg = CCP_SetPhyDqTimingReg,
        .getPhyDqTimingReg = CCP_GetPhyDqTimingReg,
        .setPhyDqsTimingReg = CCP_SetPhyDqsTimingReg,
        .getPhyDqsTimingReg = CCP_GetPhyDqsTimingReg,
        .setPhyGateLpbkCtrlReg = CCP_SetPhyGateLpbkCtrlReg,
        .getPhyGateLpbkCtrlReg = CCP_GetPhyGateLpbkCtrlReg,
        .setPhyDllMasterCtrlReg = CCP_SetPhyDllMasterCtrlReg,
        .getPhyDllMasterCtrlReg = CCP_GetPhyDllMasterCtrlReg,
        .setPhyDllSlaveCtrlReg = CCP_SetPhyDllSlaveCtrlReg,
        .getPhyDllSlaveCtrlReg = CCP_GetPhyDllSlaveCtrlReg,
        .setPhyIeTimingReg = CCP_SetPhyIeTimingReg,
        .getPhyIeTimingReg = CCP_GetPhyIeTimingReg,
        .getPhyObsReg0 = CCP_GetPhyObsReg0,
        .getPhyDllObsReg0 = CCP_GetPhyDllObsReg0,
        .getPhyDllObsReg1 = CCP_GetPhyDllObsReg1,
        .getPhyDllObsReg2 = CCP_GetPhyDllObsReg2,
        .setPhyStaticToggReg = CCP_SetPhyStaticToggReg,
        .getPhyStaticToggReg = CCP_GetPhyStaticToggReg,
        .setPhyWrRdDeskewCmd = CCP_SetPhyWrRdDeskewCmd,
        .getPhyWrRdDeskewCmd = CCP_GetPhyWrRdDeskewCmd,
        .setPhyWrDeskewPdCtrl0Dq0 = CCP_SetPhyWrDeskewPdCtrl0Dq0,
        .getPhyWrDeskewPdCtrl0Dq0 = CCP_GetPhyWrDeskewPdCtrl0Dq0,
        .setPhyCtrlMain = CCP_SetPhyCtrlMain,
        .getPhyCtrlMain = CCP_GetPhyCtrlMain,
        .setPhyTselReg = CCP_SetPhyTselReg,
        .getPhyTselReg = CCP_GetPhyTselReg,
        .setPhyGpioCtrl0 = CCP_SetPhyGpioCtrl0,
        .setPhyGpioCtrl1 = CCP_SetPhyGpioCtrl1,
        .getPhyGpioStatus0 = CCP_GetPhyGpioStatus0,
        .getPhyGpioStatus1 = CCP_GetPhyGpioStatus1,
        .waitForDllLock = CCP_WaitForDllLock,
        .getPhyFeaturesReg = CCP_GetPhyFeaturesReg,
    };

    return &driver;
}
