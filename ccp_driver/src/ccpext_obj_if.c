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
 * Extracted data
 **********************************************************************/

#include "ccpext_obj_if.h"

/* parasoft suppress item METRICS-41-3 "Number of blocks of comments per statement, DRV-4926" */

CCPEXT_OBJ *CCPEXT_GetInstance(void)
{
    static CCPEXT_OBJ driver =
    {
        .setPhyWrDeskewPdCtrl0Dqx = CCPEXT_SetPhyWrDeskewPdCtrl0Dqx,
        .getPhyWrDeskewPdCtrl0Dqx = CCPEXT_GetPhyWrDeskewPdCtrl0Dqx,
        .setPhyWrDeskewReg = CCPEXT_SetPhyWrDeskewReg,
        .getPhyWrDeskewReg = CCPEXT_GetPhyWrDeskewReg,
        .setPhyRdDeskewReg = CCPEXT_SetPhyRdDeskewReg,
        .getPhyRdDeskewReg = CCPEXT_GetPhyRdDeskewReg,
        .setPhyWrDeskewPdCtrl1Reg = CCPEXT_SetPhyWrDeskewPdCtrl1Reg,
        .getPhyWrDeskewPdCtrl1Reg = CCPEXT_GetPhyWrDeskewPdCtrl1Reg,
        .setPhyCtrlLowFreqSel = CCPEXT_SetPhyCtrlLowFreqSel,
        .getPhyCtrlLowFreqSel = CCPEXT_GetPhyCtrlLowFreqSel,
        .setPhyWrRdDeskewCmdExt = CCPEXT_SetPhyWrRdDeskewCmdExt,
        .getPhyWrRdDeskewCmdExt = CCPEXT_GetPhyWrRdDeskewCmdExt,
    };

    return &driver;
}
