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
 * Combo PHY configuration functions
 *****************************************************************************/

#include <stdio.h>

#include "common.h"
#include "sdio_dfi.h"
#include "csdd_if.h"
#include "csdd_structs_if.h"

extern CSDD_SDIO_Host *sdHost;
extern CCP_OBJ *ccpDriver;
extern CCPEXT_OBJ *ccpDriverExt;
extern CCP_PrivateData *ccpPD;

CSDD_CPhyConfigDfiRd dfiRd = {
    .rddataSwap = 0,
    .rddataEn = 1,
    .rdcmdEn = 1
};

static uint32_t dfiConfigure(void)
{
    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    return CSDD_CPhyDllReset(sdHost, true);
}

static uint32_t dfiRelease(void)
{
    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    return CSDD_CPhyDllReset(sdHost, false);
}

static uint32_t ccpInitPhyDqsTiming(void)
{
    uint32_t status;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    CCP_PhyDqsTimingReg config = { 0 };
    config.useExtLpbkDqs = 0x1U;
    config.useLpbkDqs = 0x1U;
    config.usePhonyDqs = 0x1U;
    config.usePhonyDqsCmd = 0x1U;
    status = ccpDriver->setPhyDqsTimingReg(ccpPD, &config);
    CHECK_STATUS(status);
    return status;
}

static uint32_t ccpInitLpbk(void)
{
    uint32_t status;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    CCP_PhyGateLpbkCtrlReg config = { 0 };
    config.syncMethod = 0x1U;
    config.swHalfCycleShift = 0x0U;
    config.rdDelSel = 52U;
    config.gateCfgAlwaysOn = 0x1U;
    status = ccpDriver->setPhyGateLpbkCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);
    return status;
}

static uint32_t ccpInitDllMaster(void)
{
    uint32_t status;
    CCP_PhyDllMasterCtrlReg config = { 0 };

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    status = ccpDriver->getPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);

    config.paramDllBypassMode = 1;
    config.paramPhaseDetectSel = 2;
    config.paramDllStartPoint = 4;
    config.paramDllLockNum = 0;
    status = ccpDriver->setPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);
    return status;
}

static uint32_t ccpInitDllSlave(void)
{
    uint32_t status;
    CCP_PhyDllSlaveCtrlReg config = { 0 };

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    config.readDqsCmdDelay = 0;
    config.clkWrdqsDelay = 0;
    config.clkWrDelay = 0;
    config.readDqsDelay = 0;
    status = ccpDriver->setPhyDllSlaveCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);
    return status;
}

static uint32_t ccpInitPhyCtrl(void)
{
    uint32_t status;
    CCP_PhyCtrlMain config = { 0 };

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    status = ccpDriver->getPhyCtrlMain(ccpPD, &config);
    CHECK_STATUS(status);

    config.phonyDqsTiming = 0;

    status = ccpDriver->setPhyCtrlMain(ccpPD, &config);
    CHECK_STATUS(status);
    return status;
}

static uint32_t ccpInitPhyDqTiming(void)
{
    uint32_t status;
    CCP_PhyDqTimingReg config = { 0 };

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    status = ccpDriver->getPhyDqTimingReg(ccpPD, &config);
    CHECK_STATUS(status);

    config.ioMaskAlwaysOn = 0;
    config.ioMaskEnd = 0;
    config.ioMaskStart = 0;
    // This field must be set to at least the value of bits [6:4]+2 to prevent disabling the pad before the data is completely written.
    config.dataSelectOeEnd = 2;

    status = ccpDriver->setPhyDqTimingReg(ccpPD, &config);
    CHECK_STATUS(status);
    return status;
}

uint32_t dfiInit(void)
{
    uint32_t status = CDN_EOK;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();
    ccpInitDllMaster();
    ccpInitDllSlave();
    ccpInitPhyCtrl();
    status = dfiRelease();
    CHECK_STATUS(status);
    ccpInitPhyDqTiming();

    status = CSDD_SetCPhyExtMode(sdHost, true, true);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 1);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata0Dly = 1;
    outputDelay.wrcmd0Dly = 1;
    CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 9;
    CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);

    return status;
}

uint32_t dfisdr12Init(void)
{
    uint32_t status = CDN_EOK;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();
    ccpInitDllMaster();
    ccpInitDllSlave();
    ccpInitPhyCtrl();

    status = dfiRelease();
    CHECK_STATUS(status);
    ccpInitPhyDqTiming();

    status = CSDD_SetCPhyExtMode(sdHost, true, true);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 2);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    // with zero, timing violation is coming
    outputDelay.wrdata0Dly = 1;
    outputDelay.wrcmd0Dly = 1;
    status = CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);
    CHECK_STATUS(status);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 9;
    ioDelay.idelayVal = 1;
    status = CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);
    CHECK_STATUS(status);
    return status;
}

uint32_t dfisdr25Init(void)
{
    uint32_t status = CDN_EOK;
    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();

    CCP_PhyDllMasterCtrlReg config = { 0 };
    status = ccpDriver->getPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);

    config.paramDllBypassMode = 1;
    config.paramPhaseDetectSel = 2;
    config.paramDllStartPoint = 4;
    /* While working with Cadence SD Host Controller,
     * this field is not applicable and must be set to
     * its default (reset) value. */
    config.paramDllLockNum = 0;

    status = ccpDriver->setPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);
    ccpInitDllSlave();
    ccpInitPhyCtrl();
    status = dfiRelease();
    CHECK_STATUS(status);
    ccpInitPhyDqTiming();

    status = CSDD_SetCPhyExtMode(sdHost, true, true);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 3);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata0Dly = 1;
    outputDelay.wrcmd0Dly = 1;
    status = CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);
    CHECK_STATUS(status);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 9;
    ioDelay.idelayVal = 1;
    status = CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);
    CHECK_STATUS(status);
    return status;
}

uint32_t dfisdr104Init(void) {
    uint32_t status = CDN_EOK;
    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }
    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();

    CCP_PhyDllMasterCtrlReg config = { 0 };
    status = ccpDriver->getPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);

    config.paramDllBypassMode = 0;
    config.paramPhaseDetectSel = 2;
    config.paramDllStartPoint = 4;
    config.paramDllLockNum = 0; //While working with Cadence SD Host Controller, this field is not applicable and must be set to its default (reset) value.

    status = ccpDriver->setPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);

    CCP_PhyDllSlaveCtrlReg config1 = { 0 };
    config1.readDqsCmdDelay = 0;
    config1.clkWrdqsDelay = 91;
    config1.clkWrDelay = 91;
    config1.readDqsDelay = 0;
    status = ccpDriver->setPhyDllSlaveCtrlReg(ccpPD, &config1);
    CHECK_STATUS(status);

    status = dfiRelease();
    CHECK_STATUS(status);

    CCP_PhyDqTimingReg configdq = { 0 };
    status = ccpDriver->getPhyDqTimingReg(ccpPD, &configdq);
    CHECK_STATUS(status);

    configdq.ioMaskAlwaysOn = 0;
    configdq.ioMaskEnd = 0;
    configdq.ioMaskStart = 1;
    /* This field must be set to at least the value of bits [6:4]+2 to prevent
       disabling the pad before the data is completely written.*/
    configdq.dataSelectOeEnd = 2;

    status = ccpDriver->setPhyDqTimingReg(ccpPD, &configdq);
    CHECK_STATUS(status);

    status = CSDD_SetCPhyExtMode(sdHost, false, false);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 8);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    // Must be zero Hold violation
    outputDelay.wrdata0Dly = 0;
    outputDelay.wrcmd0Dly = 0;
    status = CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);
    CHECK_STATUS(status);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 9;
    // Must be zero for SDR104
    ioDelay.idelayVal = 0;
    status = CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);
    CHECK_STATUS(status);
    return status;
}
uint32_t dfisdr50Init(void)
{
    uint32_t status = CDN_EOK;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();
    ccpInitDllMaster();
    ccpInitDllSlave();
    ccpInitPhyCtrl();
    dfiRelease();
    ccpInitPhyDqTiming();

    status = CSDD_SetCPhyExtMode(sdHost, true, true);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 5);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata0Dly = 0;
    outputDelay.wrcmd0Dly = 0;
    CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 8;
    CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);

    return status;
}

uint32_t dfisdrInit(void)
{
    uint32_t status = CDN_EOK;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();
    ccpInitDllMaster();
    ccpInitDllSlave();
    status = dfiRelease();
    CHECK_STATUS(status);
    ccpInitPhyDqTiming();

    status = CSDD_SetCPhyExtMode(sdHost, true, true);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 2);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata0Dly = 1;
    outputDelay.wrcmd0Dly = 1;
    CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 9;
    CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);

    return status;
}

uint32_t dfiddrInit(void)
{
    uint32_t status = CDN_EOK;


    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();
    ccpInitDllMaster();
    ccpInitDllSlave();
    status = dfiRelease();
    CHECK_STATUS(status);

    CCP_PhyDqTimingReg config = { 0 };
    status = ccpDriver->getPhyDqTimingReg(ccpPD, &config);
    CHECK_STATUS(status);

    config.ioMaskAlwaysOn = 0;
    config.ioMaskEnd = 0;
    config.ioMaskStart = 0;
    config.dataSelectOeEnd = 1;

    status = ccpDriver->setPhyDqTimingReg(ccpPD, &config);
    CHECK_STATUS(status);

    status = CSDD_SetCPhyExtMode(sdHost, true, true);
    CHECK_STATUS(status);

    status = CSDD_SetCPhyExtMode(sdHost, true, true);
    CHECK_STATUS(status);

    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 2);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata1SdclkDly = 1;
    outputDelay.wrdata0SdclkDly = 1;
    outputDelay.wrcmd0Dly = 1;
    CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 8;
    CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);

    return status;
}

uint32_t dfihs200Init(uint32_t tune_val)
{
    uint32_t status = CDN_EOK;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    tune_val = 0xff & ((tune_val << 8) / 40); // N * 256 / 40
    dfiConfigure();
    ccpInitPhyDqsTiming();
    ccpInitLpbk();

    CCP_PhyDllMasterCtrlReg config = { 0 };
    status = ccpDriver->getPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);

    config.paramDllBypassMode = 0;
    config.paramPhaseDetectSel = 2;
    config.paramDllStartPoint = 4;
    config.paramDllLockNum = 0;
    status = ccpDriver->setPhyDllMasterCtrlReg(ccpPD, &config);
    CHECK_STATUS(status);

    CCP_PhyDllSlaveCtrlReg configs = { 0 };
    configs.readDqsCmdDelay = tune_val;
    configs.clkWrdqsDelay = 91;
    configs.clkWrDelay = 91;
    configs.readDqsDelay = tune_val;
    status = ccpDriver->setPhyDllSlaveCtrlReg(ccpPD, &configs);
    CHECK_STATUS(status);

    status = dfiRelease();
    CHECK_STATUS(status);

    CCP_PhyDqTimingReg config_timing = { 0 };
    status = ccpDriver->getPhyDqTimingReg(ccpPD, &config_timing);
    CHECK_STATUS(status);

    config_timing.ioMaskAlwaysOn = 0;
    config_timing.ioMaskEnd = 0;
    config_timing.ioMaskStart = 0;
    config_timing.dataSelectOeEnd = 1;

    status = ccpDriver->setPhyDqTimingReg(ccpPD, &config_timing);
    CHECK_STATUS(status);

    status = CSDD_SetCPhyExtMode(sdHost, false, false);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 8);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata0Dly = 1;
    outputDelay.wrcmd0Dly = 1;
    CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 9;
    CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);

    return status;
}

uint32_t dfihs400Init(void)
{
    uint32_t status = CDN_EOK;
    CCP_PhyDllSlaveCtrlReg config_slave = { 0 };

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    status = ccpDriver->getPhyDllSlaveCtrlReg(ccpPD, &config_slave);
    CHECK_STATUS(status);

    dfiConfigure();

    CCP_PhyDqsTimingReg config = { 0 };
    config.useExtLpbkDqs = 0x1U;
    config.useLpbkDqs = 0x1U;
    config.usePhonyDqs = 0x0U;
    config.usePhonyDqsCmd = 0x1U;
    status = ccpDriver->setPhyDqsTimingReg(ccpPD, &config);
    CHECK_STATUS(status);

    ccpInitLpbk();

    CCP_PhyDllMasterCtrlReg config_master = { 0 };
    status = ccpDriver->getPhyDllMasterCtrlReg(ccpPD, &config_master);
    CHECK_STATUS(status);

    config_master.paramDllBypassMode = 0;
    config_master.paramPhaseDetectSel = 2;
    config_master.paramDllStartPoint = 4;
    config_master.paramDllLockNum = 0;
    status = ccpDriver->setPhyDllMasterCtrlReg(ccpPD, &config_master);
    CHECK_STATUS(status);

    config_slave.clkWrdqsDelay = 91;
    config_slave.clkWrDelay = 89;
    config_slave.readDqsDelay = 64;
    status = ccpDriver->setPhyDllSlaveCtrlReg(ccpPD, &config_slave);
    CHECK_STATUS(status);

    status = dfiRelease();
    CHECK_STATUS(status);

    CCP_PhyDqTimingReg config_timing = { 0 };
    status = ccpDriver->getPhyDqTimingReg(ccpPD, &config_timing);
    CHECK_STATUS(status);

    config_timing.ioMaskAlwaysOn = 0;
    config_timing.ioMaskEnd = 1;
    config_timing.ioMaskStart = 0;
    config_timing.dataSelectOeEnd = 2;

    status = ccpDriver->setPhyDqTimingReg(ccpPD, &config_timing);
    CHECK_STATUS(status);

    status = CSDD_SetCPhyExtMode(sdHost, false, false);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 9);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata1SdclkDly = 1;
    outputDelay.wrdata0SdclkDly = 1;
    outputDelay.wrcmd0Dly = 1;
    CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 8;
    CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);

    return status;
}

uint32_t dfihs400esInit(void)
{
    uint32_t status = CDN_EOK;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();

    CCP_PhyDqsTimingReg config = { 0 };
    config.useExtLpbkDqs = 0x1U;
    config.useLpbkDqs = 0x1U;
    config.usePhonyDqs = 0x0U;
    config.usePhonyDqsCmd = 0x0U;
    status = ccpDriver->setPhyDqsTimingReg(ccpPD, &config);
    CHECK_STATUS(status);

    ccpInitLpbk();

    CCP_PhyDllMasterCtrlReg config_master = { 0 };
    status = ccpDriver->getPhyDllMasterCtrlReg(ccpPD, &config_master);
    CHECK_STATUS(status);

    config_master.paramDllBypassMode = 0;
    config_master.paramPhaseDetectSel = 2;
    config_master.paramDllStartPoint = 4;
    config_master.paramDllLockNum = 0;
    status = ccpDriver->setPhyDllMasterCtrlReg(ccpPD, &config_master);
    CHECK_STATUS(status);

    CCP_PhyDllSlaveCtrlReg config_slave = { 0 };
    config_slave.readDqsCmdDelay = 64;
    config_slave.clkWrdqsDelay = 91;
    config_slave.clkWrDelay = 89;
    config_slave.readDqsDelay = 64;
    status = ccpDriver->setPhyDllSlaveCtrlReg(ccpPD, &config_slave);
    CHECK_STATUS(status);

    status = dfiRelease();
    CHECK_STATUS(status);

    CCP_PhyDqTimingReg config_timing = { 0 };
    status = ccpDriver->getPhyDqTimingReg(ccpPD, &config_timing);
    CHECK_STATUS(status);

    config_timing.ioMaskAlwaysOn = 0;
    config_timing.ioMaskEnd = 1;
    config_timing.ioMaskStart = 0;
    config_timing.dataSelectOeEnd = 2;

    status = ccpDriver->setPhyDqTimingReg(ccpPD, &config_timing);
    CHECK_STATUS(status);

    status = CSDD_SetCPhyExtMode(sdHost, false, false);
    CHECK_STATUS(status);
    status = CSDD_SetCPhyConfigDfiRd(sdHost, &dfiRd);
    CHECK_STATUS(status);

    status = CSDD_SetCPhySdclkAdj(sdHost, 9);
    CHECK_STATUS(status);

    CSDD_CPhyConfigOutputDelay outputDelay = { 0 };
    outputDelay.wrdata1SdclkDly = 1;
    outputDelay.wrdata0SdclkDly = 1;
    outputDelay.wrcmd0Dly = 1;
    CSDD_SetCPhyConfigOutputDelay(sdHost, &outputDelay);

    CSDD_CPhyConfigIoDelay ioDelay = {0};
    ioDelay.rwCompensate = 8;
    CSDD_SetCPhyConfigIoDelay(sdHost, &ioDelay);

    return status;
}

uint32_t dfihs400InitPreCond()
{
    uint32_t status = CDN_EOK;

    if (sdHost->hostCtrlVer < 6) {
        return 0;
    }

    dfiConfigure();

    CCP_PhyDllSlaveCtrlReg config_slave = { 0 };
    status = ccpDriver->getPhyDllSlaveCtrlReg(ccpPD, &config_slave);
    CHECK_STATUS(status);
    config_slave.clkWrdqsDelay = 192;
    config_slave.clkWrDelay = 64;
    status = ccpDriver->setPhyDllSlaveCtrlReg(ccpPD, &config_slave);
    CHECK_STATUS(status);

    status = dfiRelease();
    CHECK_STATUS(status);
    return status;
}

