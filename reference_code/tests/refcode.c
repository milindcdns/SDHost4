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
 * Reference code showing how to use the driver and used as functional tests
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cdn_log.h>

#include <test_harness_log.h>
#include <map_system_memory.h>

#include <csp.h>
#include <csdd_obj_if.h>
#include <csdd_structs_if.h>
#include <sdio_dfi.h>
#include <irq.h>
#include <common.h>

uint32_t g_dbg_enable_log;
uint32_t g_dbg_log_lvl;
uint32_t g_dbg_log_cnt;
uint32_t g_dbg_state;

/* This should be the base address of
   the Cadence IP core on your system */
#define SD_REG_BASE SDHC0_REGS_APB_BASE

#define MAX_PHY_INPUT_DELAY 31
#define IDLE() CPS_DelayNs(100U)

extern void testResult(char* testName, uint32_t errorCode);


CSDD_Config config;
CSDD_Callbacks cb;

CSDD_OBJ *sdHostDriver;
CSDD_SDIO_Host *sdHost;

CCP_OBJ *ccpDriver;
CCPEXT_OBJ *ccpDriverExt;
CCP_PrivateData *ccpPD;

unsigned int clockFreqsKHz[] = { 1600, 3200, 6300, 12500, 25000 };
unsigned char busWidthArray[] = {CSDD_BUS_WIDTH_1, CSDD_BUS_WIDTH_4,
                                 CSDD_BUS_WIDTH_8};

uint8_t setTuneVal(const CSDD_SDIO_Host* pd, uint8_t tune_val)
{

    return dfihs200Init(tune_val);
}

uint8_t WriteReadCompare(uint8_t slotIndex, uint32_t sectorNumber,
                         uint32_t DataSize)
{
    uint8_t status;

    SubPrint("\tWrite data to memory sector number:%d, data size:%d\n",
             sectorNumber, DataSize);

    status = sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex,
                                                  sectorNumber, writeBuffer,
                                                  DataSize,
                                                  CSDD_TRANSFER_WRITE);
    CHECK_STATUS(status);

    SubPrint("\tData write operation succeed\n");

    Clearbuf(readBuffer, DataSize, 0xDEADBEEF);

    SubPrint("\tRead data from memory sector number:%d, data size:%d\n",
             sectorNumber, DataSize);

    status = sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex,
                                                  sectorNumber, readBuffer,
                                                  DataSize,
                                                  CSDD_TRANSFER_READ);

    CHECK_STATUS(status);
    SubPrint("\tData read operation succeed\n");

    status = Comparebuf(writeBuffer, readBuffer, DataSize);
    if (status) {
        SubPrint("\tError written data and read data are different\n\n");
    }
    else{
        SubPrint("\tWritten data and read data are identically\n\n");
    }
    return status;
}

uint8_t InfiniteWriteTest(uint8_t slotIndex, uint32_t sectorNumber,
                          uint32_t DataSize, uint32_t Count)
{
    uint8_t status;
    uint32_t i;
    uint8_t *wbuf = writeBuffer;
    volatile int delay = 0x1FFFF;

    SubPrint("\tWrite data to memory sector number:%d, data size:%d\n",
             sectorNumber, DataSize);

    Clearbuf(readBuffer, DataSize * Count, 0xDEADBEEF);

    status = sdHostDriver->memCardInfXferStart(sdHost, slotIndex,
                                                sectorNumber, wbuf, DataSize,
                                                CSDD_TRANSFER_WRITE);
    CHECK_STATUS(status);
    wbuf += DataSize;

    for (i = 0; i < Count - 1; i++) {
        status = sdHostDriver->memCardInfXferContinue(sdHost, slotIndex, wbuf,
                                                      DataSize,
                                                      CSDD_TRANSFER_WRITE);
        CHECK_STATUS(status);
        wbuf += DataSize;

    }

    /* wait until memory card finish programing flash */
    while (delay--) {; }

    status = sdHostDriver->memCardInfXferFinish(sdHost, slotIndex,
                                                CSDD_TRANSFER_WRITE);
    CHECK_STATUS(status);

    SubPrint("\tInfinite Data write operation succeed\n");

    status = sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex,
                                                  sectorNumber, readBuffer,
                                                  DataSize * Count,
                                                  CSDD_TRANSFER_READ);

    CHECK_STATUS(status);
    SubPrint("\tData read operation succeed\n");

    status = Comparebuf(writeBuffer, readBuffer, DataSize * Count);
    if (status) {
        SubPrint("\tError written data and read data are different\n\n");
    }
    else{
        SubPrint("\tritten data and read data are identically\n\n");
    }
    return status;
}

uint8_t InfiniteReadTest(uint8_t slotIndex, uint32_t sectorNumber,
                         uint32_t DataSize, uint32_t Count)
{
    uint8_t status;
    uint32_t i;
    uint8_t *rbuf = readBuffer;

    SubPrint("\tWrite data to memory sector number:%d, data size 2048 bytes\n",
             sectorNumber);

    status = sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex,
                                                  sectorNumber, writeBuffer,
                                                  DataSize * Count,
                                                  CSDD_TRANSFER_WRITE);
    CHECK_STATUS(status);

    SubPrint("\tData write operation succeed\n");

    Clearbuf(readBuffer, DataSize * Count, 0xDEADBEEF);

    SubPrint("\tRead data from memory sector number:%d, data size:%d\n",
             sectorNumber, DataSize);

    status = sdHostDriver->memCardInfXferStart(sdHost, slotIndex, sectorNumber,
                                               rbuf, DataSize,
                                               CSDD_TRANSFER_READ);
    CHECK_STATUS(status);
    rbuf += DataSize;

    for (i = 0; i < Count - 1; i++) {
        status = sdHostDriver->memCardInfXferContinue(sdHost, slotIndex, rbuf,
                                                      DataSize,
                                                      CSDD_TRANSFER_READ);
        CHECK_STATUS(status);
        rbuf += DataSize;
    }
    status = sdHostDriver->memCardInfXferFinish(sdHost, slotIndex,
                                                CSDD_TRANSFER_READ);
    CHECK_STATUS(status);

    SubPrint("\tData read operation succeed\n");

    status = Comparebuf(writeBuffer, readBuffer, DataSize * Count);
    if (status) {
        SubPrint("\tError written data and read data are different\n\n");
    }
    else{
        SubPrint("\tWritten data and read data are identically\n\n");
    }
    return status;
}

uint8_t BusWidthTest(uint8_t slotIndex, uint32_t sectorNumber,
                     uint8_t busWidthArray[], uint8_t busWidthArraySize)
{
    int i;
    uint8_t status;

    for (i = 1; i < busWidthArraySize; i++) {
        unsigned char busWidth = busWidthArray[i];
        unsigned char size = sizeof(busWidth);

        /* change bus width */
        status = sdHostDriver->configure(sdHost, slotIndex,
                                         CSDD_CONFIG_SET_BUS_WIDTH,
                                         &busWidth, &size);
        if (status) {
            SubPrint("\tSetting bus width to %d failed\n\n", busWidth);
            continue;
        }

        status = WriteReadCompare(slotIndex, sectorNumber, 2048);
        CHECK_STATUS(status);

        sectorNumber += 4;
    }

    return 0;
}

uint8_t DMATest(uint8_t slotIndex, uint32_t sectorNumber)
{
    int i;
    uint8_t status, DmaMode, size;

    uint8_t DmaModeArray[] = {
        CSDD_NONEDMA_MODE
#if SDIO_SDMA_SUPPORTED
        ,CSDD_SDMA_MODE
#endif
#if SDIO_ADMA2_SUPPORTED
        ,CSDD_ADMA2_MODE
#endif
#if SDIO_ADMA3_SUPPORTED
        ,CSDD_ADMA3_MODE
#endif
    };

    for (i = 0; i < sizeof(DmaModeArray); i++) {
        DmaMode = DmaModeArray[i];
        size = sizeof(DmaMode);

        /* change DMA mode */
        status = sdHostDriver->configure(sdHost, slotIndex,
                                         CSDD_CONFIG_SET_DMA_MODE,
                                         &DmaMode, &size);
        if (status) {
            SubPrint("\tSetting DMA mode to %d failed\n\n", DmaMode);
            continue;
        }
        SubPrint("\tSetting DMA mode to %d succeed\n", DmaMode);

        status = WriteReadCompare(slotIndex, sectorNumber, 2048);
        CHECK_STATUS(status);

        sectorNumber += 4;
    }

    DmaMode = CSDD_AUTO_MODE;
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_DMA_MODE,
                                     &DmaMode, &size);
    if (status) {
        SubPrint("\tSetting DMA mode to %d failed\n\n", DmaMode);
    }

    return 0;
}

uint8_t ADMA3Test(uint8_t slotIndex, uint32_t sectorNumber)
{
    uint8_t status;
    uint8_t DmaMode = CSDD_ADMA3_MODE;
    uint8_t size;
    CSDD_Request Request = {0};
    uint32_t DataSize;
    uint16_t rca;
    if (sdHost->hostCtrlVer < 6) {
        printf("ADMA3 is not supported for host %d\n", sdHost->hostCtrlVer);
        return CDN_ENOTSUP;
    }
    /* command one, first command must be a data transfer command */
    Request.pCmd[0].command = 25;
    Request.pCmd[0].argument = sectorNumber;
    Request.pCmd[0].requestFlags.dataPresent = 1;
    Request.pCmd[0].requestFlags.dataTransferDirection = CSDD_TRANSFER_WRITE;
    Request.pCmd[0].requestFlags.hwResponseCheck = 1;
    Request.pCmd[0].requestFlags.responseType = CSDD_RESPONSE_R1;
    Request.pCmd[0].requestFlags.commandType = CSDD_CMD_TYPE_NORMAL;
    Request.pCmd[0].blockCount = 2;
    Request.pCmd[0].blockLen = 512;
    Request.pCmd[0].subBuffersCount = 0;
    Request.pCmd[0].pDataBuffer = writeBuffer;

    status = sdHostDriver->getRca(sdHost, 0, &rca);
    if (status) {
        printf("error cannot get RCA address\n");
        return 1;
    }

    /* command two */
    Request.pCmd[1].command = 13;
    Request.pCmd[1].argument = (uint32_t)rca << 16;
    Request.pCmd[1].requestFlags.dataPresent = 0;
    Request.pCmd[1].requestFlags.hwResponseCheck = 1;
    Request.pCmd[1].requestFlags.commandType = CSDD_CMD_TYPE_NORMAL;
    Request.pCmd[1].requestFlags.responseType = CSDD_RESPONSE_R1;

    /* command three */
    Clearbuf(readBuffer, 8192, 0xDEADBEEF);
    Request.pCmd[2].command = 18;
    Request.pCmd[2].argument = sectorNumber;
    Request.pCmd[2].requestFlags.dataPresent = 1;
    Request.pCmd[2].requestFlags.dataTransferDirection = CSDD_TRANSFER_READ;
    Request.pCmd[2].requestFlags.hwResponseCheck = 1;
    Request.pCmd[2].requestFlags.responseType = CSDD_RESPONSE_R1;
    Request.pCmd[2].requestFlags.commandType = CSDD_CMD_TYPE_NORMAL;
    Request.pCmd[2].blockCount = 2;
    Request.pCmd[2].blockLen = 512;
    Request.pCmd[2].subBuffersCount = 0;
    Request.pCmd[2].pDataBuffer = readBuffer;

    Request.cmdCount = 3;
    Request.busyCheckFlags = 0;
    Request.commandCategory = CSDD_CMD_CAT_NORMAL;
    Request.requestType = (uint8_t)CSDD_REQUEST_TYPE_SD;

    /* change DMA mode */
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_DMA_MODE,
                                     &DmaMode, &size);
    if (status == CDN_EOK) {
        SubPrint("\tSetting DMA mode to %d Succeed\n\n", DmaMode);
        sdHostDriver->execCardCommand(sdHost, slotIndex, &Request);
        sdHostDriver->waitForRequest(Request.pSdioHost, &Request);
        if (Request.status == CDN_EOK) {
            DataSize = Request.pCmd[0].blockCount * Request.pCmd[0].blockLen;
            status = Comparebuf(writeBuffer, readBuffer, DataSize);
            if (status) {
                SubPrint("\tError written data and read data are different\n\n");
            }
            else {
                SubPrint("\tWritten data and read data are identically\n\n");
            }
        }
        CHECK_STATUS(status);
    } else {
        SubPrint("\tSetting DMA mode to %d Failed\n\n", DmaMode);
    }

    DmaMode = CSDD_AUTO_MODE;
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_DMA_MODE,
                                     &DmaMode, &size);
    if (status) {
        SubPrint("\tSetting DMA mode to %d failed\n\n", DmaMode);
    }
    return status;
}

uint8_t SingleSectorTest(uint8_t slotIndex, uint32_t sectorNumber)
{
    uint8_t status, i;

    for (i = 0; i < 4; i++) {
        status = WriteReadCompare(slotIndex, sectorNumber, 512);
        CHECK_STATUS(status);
    }

    return 0;
}

uint8_t NonBlockingTest(uint8_t slotIndex, uint32_t sectorNumber)
{
    uint8_t status;
    void *pReq;
    uint32_t DataSize = 8192;

    SubPrint("\tWrite data to memory sector number:%d, data size:%d\n",
             sectorNumber, DataSize);

    status = sdHostDriver->memCardDataXferNonBlock(sdHost, slotIndex,
                                                    sectorNumber,
                                                    writeBuffer, DataSize,
                                                    CSDD_TRANSFER_WRITE, &pReq);
    CHECK_STATUS(status);

    status = sdHostDriver->memCardFinishXferNonBlock(sdHost, pReq);
    CHECK_STATUS(status);

    SubPrint("\tData write operation succeed\n");

    Clearbuf(readBuffer, DataSize, 0xDEADBEEF);

    SubPrint("\tRead data from memory sector number:%d, data size:%d\n",
             sectorNumber, DataSize);

    status = sdHostDriver->memCardDataXferNonBlock(sdHost, slotIndex,
                                                    sectorNumber,
                                                    readBuffer, DataSize,
                                                    CSDD_TRANSFER_READ, &pReq);
    CHECK_STATUS(status);

    status = sdHostDriver->memCardFinishXferNonBlock(sdHost, pReq);
    CHECK_STATUS(status);
    SubPrint("\tData read operation succeed\n");

    status = Comparebuf(writeBuffer, readBuffer, DataSize);
    if (status) {
        SubPrint("\tError written data and read data are different\n\n");
    }
    else{
        SubPrint("\tWritten data and read data are identically\n\n");
    }
    return status;
}

uint8_t LowClockFreqTest(uint8_t slotIndex, uint32_t sectorNumber)
{
    int i;
    uint8_t status;
    CSDD_DeviceState deviceState;
    uint32_t freq = 5000;
    unsigned char size = sizeof(freq);

    sdHostDriver->getDeviceState(sdHost, slotIndex, &deviceState);

    CSDD_SpeedMode cardMode = (deviceState.deviceType == CSDD_CARD_TYPE_MMC)
                              ? CSDD_ACCESS_MODE_MMC_LEGACY :
                              CSDD_ACCESS_MODE_SDR12;

    status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                     &freq, &size);
    CHECK_STATUS(status);

    /* set card mode */
    status = sdHostDriver->configureAccessMode(sdHost, slotIndex, cardMode);
    CHECK_STATUS(status);

    printf("start SD test\n");
    for (i = 0; i < sizeof(clockFreqsKHz) / sizeof(int); i++) {
        freq = clockFreqsKHz[i];
        size = sizeof(freq);

        SubPrint("\tTry set host clock to %dKHz...\n", freq);
        status = sdHostDriver->configure(sdHost, slotIndex,
                                         CSDD_CONFIG_SET_CLK, &freq, &size);
        CHECK_STATUS(status);
        SubPrint("\tHost clock freq changed and equals now %dKHz\n\n", freq);

        status = WriteReadCompare(slotIndex, sectorNumber, 2048);
        CHECK_STATUS(status);

        sectorNumber += 4;
    }
    return 0;
}

struct PhyTimingMode {
    uint32_t clockFreqsKHz;
    CSDD_SpeedMode cardMode;
    char *modeText;
    CSDD_PhyDelay phyDelay;
};

static uint8_t SetMode(uint8_t slotIndex, struct PhyTimingMode *mode)
{
    uint8_t status;
    unsigned char size;
    uint32_t freq = 5000;

    /* set card mode */
    status = sdHostDriver->configureAccessMode(sdHost, slotIndex,
                                               mode->cardMode);
    if (status == ENOTSUP) {
        SubPrint("\t %s mode is not supported by a card\n", mode->modeText);
        return status;
    }

    CHECK_STATUS(status);
    /* set new clock frequency */
    freq = mode->clockFreqsKHz;
    size = sizeof(freq);
    status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                     &freq, &size);
    CHECK_STATUS(status);

    return 0;
}

uint8_t ReinitHostAndDevice(void)
{
    uint8_t status;
    printf("Reinitialize SD Host and device\n");

    /* reset host */
    status = sdHostDriver->resetHost(sdHost);
    CHECK_STATUS(status);

    /* Initialize host */
    status = sdHostDriver->init(sdHost, &config, &cb);
    CHECK_STATUS(status);

    status = dfiInit();
    CHECK_STATUS(status);

    if (sdHost->hostCtrlVer == 4) {
        status = sdHostDriver->writePhySet(sdHost, 0,
                                           CSDD_PHY_DELAY_DLL_HS_SDCLK, 5);
        if (status) {
            printf("Error cannot set phy configuration\n");
            return status;
        }
    }

    sdHostDriver->start(sdHost);

    sdHostDriver->memoryCardLoadDriver(sdHost);

    status = sdHostDriver->checkSlots(sdHost);
    CHECK_STATUS(status);

    return 0;
}

uint8_t DoPhyTraining(uint8_t slotIndex, uint8_t *newDelay,
                      struct PhyTimingMode *mode,
                      unsigned char busWidth)
{
    int i;
    uint8_t status;
    uint8_t pos, length, currLength;
    unsigned char size;
    uint8_t maxDelay;
    uint32_t baseClk;

    printf("Start training\n");

    status = sdHostDriver->getBaseClk(sdHost, slotIndex, &baseClk);
    CHECK_STATUS(status);

    maxDelay = (baseClk / mode->clockFreqsKHz) * 2;

    size = sizeof(busWidth);
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_BUS_WIDTH,
                                     &busWidth, &size);
    CHECK_STATUS(status);

    status = SetMode(slotIndex, mode);
    /* if timing mode is not supported just return no error
       PHY training cannot be executed */
    if (status == ENOTSUP) {
        return 0;
    }
    CHECK_STATUS(status);

    pos = length = currLength = 0;
    for (i = 0; i < maxDelay; i++) {
        status = sdHostDriver->writePhySet(sdHost, slotIndex, mode->phyDelay, i);
        if (status) {
            printf("Error cannot set phy configuration\n");
            return status;
        }

        status = sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex, 0,
                                                      readBuffer,
                                                      512, CSDD_TRANSFER_READ);
        if (status == 0) {
            currLength++;
            if (currLength > length) {
                pos = i - length;
                length++;
            }
        }
        else {
            currLength = 0;
        }
#ifdef PHY_TRAINING_RESET_ON_STEP_FAIL
        if (status) {
            status = ReinitHostAndDevice();
            CHECK_STATUS(status);

            size = sizeof(busWidth);
            status = sdHostDriver->configure(sdHost, slotIndex,
                                             CSDD_CONFIG_SET_BUS_WIDTH,
                                             &busWidth, &size);
            CHECK_STATUS(status);

            status = SetMode(slotIndex, mode);
            CHECK_STATUS(status);
        }
#endif
    }

    *newDelay = pos + length / 2;
    sdHostDriver->writePhySet(sdHost, slotIndex, mode->phyDelay, *newDelay);

    return sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex, 0,
                                                readBuffer, 512,
                                                CSDD_TRANSFER_READ);
}

uint8_t PhyTraining(uint8_t slotIndex, struct PhyTimingMode *modes,
                    unsigned char busWidth, uint8_t modesCount)
{
    uint8_t status;
    int i;
    unsigned char size;
    uint32_t freq = 5000;
    uint8_t newDelay;
    uint32_t datTimeoutUs;

    freq = 5000;
    size = sizeof(freq);
    status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                     &freq, &size);
    CHECK_STATUS(status);

    datTimeoutUs = 100;
    size = sizeof(datTimeoutUs);
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_DAT_TIMEOUT,
                                     &datTimeoutUs, &size);

    for (i = 0; i < modesCount; i++) {
        status = DoPhyTraining(slotIndex, &newDelay, &modes[i], busWidth);
        CHECK_STATUS(status);
        SubPrint("PHY input delay for mode: %s and clk %d is %d\n",
                 modes[i].modeText, modes[i].clockFreqsKHz, newDelay);

    }

    datTimeoutUs = 500000;
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_DAT_TIMEOUT,
                                     &datTimeoutUs, &size);
    CHECK_STATUS(status);

    return 0;
}

uint8_t PhyTrainingMmc(uint8_t slotIndex)
{
    uint8_t modeCount;
    struct PhyTimingMode modes[] = {
        {
            .clockFreqsKHz = 26000,
            .cardMode = CSDD_ACCESS_MODE_MMC_LEGACY,
            .modeText = "MMC legacy",
            .phyDelay = CSDD_PHY_DELAY_INPUT_MMC_LEGACY,
        },
        {
            .clockFreqsKHz = 52000,
            .cardMode = CSDD_ACCESS_MODE_HS_SDR,
            .modeText = "MMC HS SDR",
            .phyDelay = CSDD_PHY_DELAY_INPUT_MMC_SDR,
        },
        {
            .clockFreqsKHz = 52000,
            .cardMode = CSDD_ACCESS_MODE_HS_DDR,
            .modeText = "MMC HS DDR",
            .phyDelay = CSDD_PHY_DELAY_INPUT_MMC_DDR,
        },
    };

    modeCount = sizeof(modes) / sizeof(modes[0]);
    return PhyTraining(slotIndex, modes, CSDD_BUS_WIDTH_8, modeCount);
}

uint8_t PhyTrainingUhs(uint8_t slotIndex)
{
    uint8_t modeCount;
    struct PhyTimingMode modes[] = {
        {
            .clockFreqsKHz = 25000,
            .cardMode = CSDD_ACCESS_MODE_SDR12,
            .modeText = "SDR12",
            .phyDelay = CSDD_PHY_DELAY_INPUT_SDR12,
        },
        {
            .clockFreqsKHz = 50000,
            .cardMode = CSDD_ACCESS_MODE_SDR25,
            .modeText = "SDR25",
            .phyDelay = CSDD_PHY_DELAY_INPUT_SDR25,
        },
        {
            .clockFreqsKHz = 50000,
            .cardMode = CSDD_ACCESS_MODE_DDR50,
            .modeText = "DDR50",
            .phyDelay = CSDD_PHY_DELAY_INPUT_DDR50,
        },
        {
            .clockFreqsKHz = 100000,
            .cardMode = CSDD_ACCESS_MODE_SDR50,
            .modeText = "SDR50",
            .phyDelay = CSDD_PHY_DELAY_INPUT_SDR50,
        },
    };

    modeCount = sizeof(modes) / sizeof(modes[0]);
    return PhyTraining(slotIndex, modes, CSDD_BUS_WIDTH_4, modeCount);
}

uint8_t MmchighSpeedTest(uint8_t slotIndex, uint32_t sectorNumber)
{
    uint8_t status;
    int i;
    uint32_t clockFreqsKHz[] = {26000, 52000, 52000, 200000, 200000, 200000};
    CSDD_SpeedMode cardModes[] = {CSDD_ACCESS_MODE_MMC_LEGACY,
                                  CSDD_ACCESS_MODE_HS_SDR,
                                  CSDD_ACCESS_MODE_HS_DDR,
                                  CSDD_ACCESS_MODE_HS_200,
                                  CSDD_ACCESS_MODE_HS_400,
                                  CSDD_ACCESS_MODE_HS_400_ES};
    char *modesText[] = {"MMC legacy", "HS SDR", "HS DDR", "HS200", "HS400",
        "HS400ES"};
    unsigned char busWidth = CSDD_BUS_WIDTH_8;
    unsigned char size;
    uint32_t freq = 5000;

    size = sizeof(freq);
    status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                     &freq, &size);
    CHECK_STATUS(status);
    SubPrint("\tChange clock frequency to %dKHz \n", freq);

    size = sizeof(busWidth);
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_BUS_WIDTH,
                                     &busWidth, &size);
    CHECK_STATUS(status);

    for (i = 0; i < sizeof(clockFreqsKHz) / sizeof(int); i++) {

        if (cardModes[i] == CSDD_ACCESS_MODE_HS_400_ES) {
            status = ReinitHostAndDevice();
            CHECK_STATUS(status);

            size = sizeof(busWidth);
            status = sdHostDriver->configure(sdHost, slotIndex,
                                             CSDD_CONFIG_SET_BUS_WIDTH,
                                             &busWidth, &size);
            CHECK_STATUS(status);
        }

        if (cardModes[i] == CSDD_ACCESS_MODE_HS_400){
            status = dfihs400InitPreCond();
            CHECK_STATUS(status);
        }

        /* set card mode */
        SubPrint("\tChange access mode to %s \n", modesText[i]);
        status = sdHostDriver->configureAccessMode(sdHost, slotIndex,
                                                   cardModes[i]);
        if (status == ENOTSUP) {
            SubPrint("\t %s mode is not supported by a card\n", modesText[i]);
            continue;
        }

        CHECK_STATUS(status);

        if (cardModes[i] == CSDD_ACCESS_MODE_HS_SDR)
        {
            status = dfisdrInit();
            CHECK_STATUS(status);
        }
        if (cardModes[i] == CSDD_ACCESS_MODE_HS_DDR)
        {
            status = dfiddrInit();
            CHECK_STATUS(status);
        }

        if (cardModes[i] == CSDD_ACCESS_MODE_HS_400)
        {
            status = dfihs400Init();
            CHECK_STATUS(status);
        }

        if (cardModes[i] == CSDD_ACCESS_MODE_HS_400_ES)
        {
            status = dfihs400esInit();
            CHECK_STATUS(status);
        }

        /* set new clock frequency */
        freq = clockFreqsKHz[i];
        size = sizeof(freq);
        status = sdHostDriver->configure(sdHost, slotIndex,
                                         CSDD_CONFIG_SET_CLK, &freq, &size);
        CHECK_STATUS(status);
        SubPrint("\tChange clock frequency to %dKHz \n", freq);

        status = WriteReadCompare(slotIndex, sectorNumber, 2048);
        CHECK_STATUS(status);

        sectorNumber += 4;
    }
    return 0;
}

uint8_t SimpleTest(uint8_t slotIndex, uint32_t sectorNumber)
{
    uint8_t status;

    status = sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex, 0,
                                                   readBuffer, 512,
                                                   CSDD_TRANSFER_READ);

    status = WriteReadCompare(slotIndex, sectorNumber, 2048);
    CHECK_STATUS(status);

    return status;
}

uint8_t HSTest(uint8_t slotIndex, uint32_t sectorNumber)
{
    uint8_t status;
    uint32_t freq = 50000;
    unsigned char size;

    SubPrint("\tSwitch to high speed mode\n");
    status = sdHostDriver->configureHighSpeed(sdHost, slotIndex, 1);
    CHECK_STATUS(status);

    size = sizeof(freq);

    /* set new clock frequency */
    status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                     &freq, &size);
    CHECK_STATUS(status);
    SubPrint("\tChange clock frequency to %dKHz \n", freq);

    status = WriteReadCompare(slotIndex, sectorNumber, 2048);
    CHECK_STATUS(status);

    return status;
}

volatile int isrCounter = 0;
void Isr(void* pD)
{
    CSDD_OBJ *sdHostDriver;
    bool handled;

    isrCounter++;
    sdHostDriver = CSDD_GetInstance();
    sdHostDriver->isr(pD, &handled);
}

uint8_t MmcSwitchMode(uint8_t slotIndex, CSDD_SpeedMode cardMode,
                      unsigned char busWidth, uint32_t clockFreqsKHz)
{
    uint8_t status;
    unsigned char size;

    SubPrint("set bus width\n");
    size = sizeof(busWidth);
    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_BUS_WIDTH,
                                     &busWidth, &size);

    SubPrint("set bus access mode\n");
    status = sdHostDriver->configureAccessMode(sdHost, slotIndex, cardMode);
    CHECK_STATUS(status);

    SubPrint("set clock\n");
    size = sizeof(clockFreqsKHz);
    status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                     &clockFreqsKHz, &size);
    CHECK_STATUS(status);
    SubPrint("\tChange clock frequency to %dKHz \n",
             (unsigned int)clockFreqsKHz);

    return status;
}

uint8_t CQEnable(void)
{
    uint8_t status;
    CSDD_CQInitConfig cQConfig;

    cQConfig.sendStatBlckCount = 1;
    cQConfig.sendStatIdleTimer = 0x1000;

    status = sdHostDriver->cQEnable(sdHost, &cQConfig);
    CHECK_STATUS(status);

    return status;
}

uint8_t CQDisable(void)
{
    uint8_t status;
    status = sdHostDriver->cQDisable(sdHost);
    if (status) {
        sdHostDriver->cQAllTasksDiscard(sdHost);
    }
    CHECK_STATUS(status);

    return status;
}

uint8_t CQExecDirectCommand(bool withInt)
{
    uint8_t status;
    CSDD_CQDcmdRequest request;
    uint16_t rca;

    memset(&request, 0, sizeof(request));

    status = sdHostDriver->cQSetDirectCmdConfig(sdHost, 0);
    CHECK_STATUS(status);

    /* read card status */
    request.cmdIdx = 13;
    request.responseType = CSDD_RESPONSE_R1;
    request.argument = 0;
    request.cmdTiming = 1;

    status = sdHostDriver->getRca(sdHost, 0, &rca);
    if (status) {
        printf("error cannot get RCA address\n");
        return 1;
    }
    request.argument = (uint32_t)rca << 16;

    status = sdHostDriver->cQExecuteDcmdRequest(sdHost, &request);
    if (status == 0) {
        printf("Error Request should not be executed because direct command is disabled.");
        return 1;
    }
    status = sdHostDriver->cQSetDirectCmdConfig(sdHost, 1);
    CHECK_STATUS(status);

    status = sdHostDriver->cQExecuteDcmdRequest(sdHost, &request);
    CHECK_STATUS(status);

    while (request.cQReqStat == CSDD_CQ_REQ_STAT_PENDING) {
        /*if interrupts are disabled then we need to call
         *  interrupt handler manually in polling mode*/
        if (!withInt) {
            bool handled;
            sdHostDriver->isr(sdHost, &handled);
        }
        IDLE();
    }

    if (request.cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) {
        printf("Executing CMD13 by direct request failed\r\n");
        return 1;
    }

    SubPrint("Response = %x\n", request.response);

    return status;
}

uint8_t CQWriteReadCompare(bool withInt)
{
    uint8_t status;

    CSDD_CQRequest request;
    CSDD_CQRequestData buffers;
    uint32_t DataSize = 4 * 512;

    memset(&request, 0, sizeof(request));

    status = sdHostDriver->cQGetUnusedTaskId(sdHost, &request.taskId);
    CHECK_STATUS(status);

    buffers.buffPhyAddr = (uintptr_t)writeBuffer;
    buffers.bufferSize = DataSize;

    request.blockAddress = 0;
    request.blockCount = 4;
    request.buffers = &buffers;
    request.numberOfBuffers = 1;
    request.transferDirection = CSDD_TRANSFER_WRITE;

    status = sdHostDriver->cQAttachRequest(sdHost, &request);
    CHECK_STATUS(status);

    status = sdHostDriver->cQStartExecuteTask(sdHost, request.taskId);
    CHECK_STATUS(status);

    while (request.cQReqStat == CSDD_CQ_REQ_STAT_PENDING) {
        /*if interrupts are disabled then we need to call
         *  interrupt handler manually in polling mode*/
        if (!withInt) {
            bool handled;
            sdHostDriver->isr(sdHost, &handled);
        }
        IDLE();
    }

    if (request.cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) {
        SubPrint("Data write using command queuing  failed\r\n");
        return 1;
    }

    Clearbuf(readBuffer, DataSize, 0xDEADBEEF);

    buffers.buffPhyAddr = (uintptr_t)readBuffer;
    request.transferDirection = CSDD_TRANSFER_READ;

    SubPrint("attach request\n");
    status = sdHostDriver->cQAttachRequest(sdHost, &request);
    CHECK_STATUS(status);

    SubPrint("execute request\n");
    status = sdHostDriver->cQStartExecuteTask(sdHost, request.taskId);
    CHECK_STATUS(status);

    while (request.cQReqStat == CSDD_CQ_REQ_STAT_PENDING) {
        /*if interrupts are disabled then we need to call
         *  interrupt handler manually in polling mode*/
        if (!withInt) {
            bool handled;
            sdHostDriver->isr(sdHost, &handled);
        }
        IDLE();
    }

    if (request.cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) {
        SubPrint("Data read using command queuing  failed\r\n");
        return 1;
    }

    status = Comparebuf(writeBuffer, readBuffer, DataSize);
    if (status) {
        SubPrint("\tError written data and read data are different\n\n");
    }
    else{
        SubPrint("\tWritten data and read data are identically\n\n");
    }
    return status;
}

uint8_t CQWriteReadCompareSplited(bool withInt, void *dataDescPtr,
                                  uint32_t dataDescSize)
{
    uint8_t status;
    CSDD_CQRequest request;
    CSDD_CQRequestData buffers[4];
    uint32_t DataSize = 4 * 512;
    int i;

    memset(&request, 0, sizeof(request));

    status = sdHostDriver->cQGetUnusedTaskId(sdHost, &request.taskId);
    CHECK_STATUS(status);

    request.taskId = 1;

    for (i = 0; i < 4; i++) {
        buffers[i].buffPhyAddr = (uintptr_t)writeBuffer + 512 * i;
        buffers[i].bufferSize = DataSize / 4;
    }

    request.descDataPhyAddr = (uintptr_t)dataDescPtr;
    request.descDataSize = dataDescSize;
    request.descDataBuffer = dataDescPtr;
    request.blockAddress = 0;
    request.blockCount = 4;
    request.buffers = &buffers[0];
    request.numberOfBuffers = 4;
    request.transferDirection = CSDD_TRANSFER_WRITE;

    status = sdHostDriver->cQAttachRequest(sdHost, &request);
    CHECK_STATUS(status);

    status = sdHostDriver->cQStartExecuteTask(sdHost, request.taskId);
    CHECK_STATUS(status);

    while (request.cQReqStat == CSDD_CQ_REQ_STAT_PENDING) {
        /*if interrupts are disabled then we need to call
         *  interrupt handler manually in polling mode*/
        if (!withInt) {
            bool handled;
            sdHostDriver->isr(sdHost, &handled);
        }
        IDLE();
    }

    if (request.cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) {
        SubPrint("Data write using command queuing  failed\r\n");
        return 1;
    }

    Clearbuf(readBuffer, DataSize, 0xDEADBEEF);

    for (i = 0; i < 4; i++) {
        buffers[i].buffPhyAddr = (uintptr_t)readBuffer + 512 * i;
        buffers[i].bufferSize = DataSize / 4;
    }

    request.transferDirection = CSDD_TRANSFER_READ;

    status = sdHostDriver->cQAttachRequest(sdHost, &request);
    CHECK_STATUS(status);

    status = sdHostDriver->cQStartExecuteTask(sdHost, request.taskId);
    CHECK_STATUS(status);

    while (request.cQReqStat == CSDD_CQ_REQ_STAT_PENDING) {
        /*if interrupts are disabled then we need to call
         *  interrupt handler manually in polling mode*/
        if (!withInt) {
            bool handled;
            sdHostDriver->isr(sdHost, &handled);
        }
        IDLE();
    }

    if (request.cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) {
        SubPrint("Data read using command queuing  failed\r\n");
        return 1;
    }

    status = Comparebuf(writeBuffer, readBuffer, DataSize);
    if (status) {
        SubPrint("\tError written data and read data are different\n\n");
    }
    else{
        SubPrint("\tWritten data and read data are identically\n\n");
    }
    return status;
}

uint8_t CQWriteReadCompareSplit2(bool withInt, bool withIntCoal)
{
    uint8_t status;
    int i;

    CSDD_CQRequest request[4];
    CSDD_CQRequestData buffers[4];
    uint32_t DataSize = 512;

    memset(&request, 0, sizeof(request));

    for (i = 0; i < 4; i++) {
        if (withIntCoal) {
            request[i].intCoalEn = 1;
        }

        request[i].taskId = i;

        buffers[i].buffPhyAddr = (uintptr_t)writeBuffer + i * DataSize;
        buffers[i].bufferSize = DataSize;

        request[i].blockAddress = i * DataSize;
        request[i].blockCount = 1;
        request[i].buffers = &buffers[i];
        request[i].numberOfBuffers = 1;
        request[i].transferDirection = CSDD_TRANSFER_WRITE;

        status = sdHostDriver->cQAttachRequest(sdHost, &request[i]);
        CHECK_STATUS(status);

    }

    status = sdHostDriver->cQStartExecuteTasks(sdHost, 0xF);
    CHECK_STATUS(status);

    while ((request[0].cQReqStat == CSDD_CQ_REQ_STAT_PENDING) ||
           (request[1].cQReqStat == CSDD_CQ_REQ_STAT_PENDING) ||
           (request[2].cQReqStat == CSDD_CQ_REQ_STAT_PENDING) ||
           (request[3].cQReqStat == CSDD_CQ_REQ_STAT_PENDING)) {
        /*if interrupts are disabled then we need to call
         *  interrupt handler manually in polling mode*/
        if (!withInt) {
            bool handled;
            sdHostDriver->isr(sdHost, &handled);
        }
        IDLE();
    }

    if ((request[0].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) ||
        (request[1].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) ||
        (request[2].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) ||
        (request[3].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED)) {
        SubPrint("Data write using command queuing  failed\r\n");
        return 1;
    }

    Clearbuf(readBuffer, DataSize, 0xDEADBEEF);

    for (i = 0; i < 4; i++) {
        buffers[i].buffPhyAddr = (uintptr_t)readBuffer + DataSize * i;
        request[i].transferDirection = CSDD_TRANSFER_READ;

        status = sdHostDriver->cQAttachRequest(sdHost, &request[i]);
        CHECK_STATUS(status);
    }

    status = sdHostDriver->cQStartExecuteTasks(sdHost, 0xF);
    CHECK_STATUS(status);

    while ((request[0].cQReqStat == CSDD_CQ_REQ_STAT_PENDING) ||
           (request[1].cQReqStat == CSDD_CQ_REQ_STAT_PENDING) ||
           (request[2].cQReqStat == CSDD_CQ_REQ_STAT_PENDING) ||
           (request[3].cQReqStat == CSDD_CQ_REQ_STAT_PENDING)) {
        /*if interrupts are disabled then we need to call
         *  interrupt handler manually in polling mode*/
        if (!withInt) {
            bool handled;
            sdHostDriver->isr(sdHost, &handled);
        }
        IDLE();
    }

    if ((request[0].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) ||
        (request[1].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) ||
        (request[2].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED) ||
        (request[3].cQReqStat != CSDD_CQ_REQ_STAT_FINISHED)) {
        SubPrint("Data read using command queuing  failed\r\n");
        return 1;
    }

    status = Comparebuf(writeBuffer, readBuffer, DataSize);
    if (status) {
        SubPrint("\tError written data and read data are different\n\n");
    }
    else{
        SubPrint("\tWritten data and read data are identically\n\n");
    }
    return status;
}

uint8_t CQInterruptCoalescingTest(bool withInt)
{
    uint8_t status;
    CSDD_CQIntCoalescingCfg intCoalCfg;
    uint32_t clockFreqKHz;

    intCoalCfg.enable = 1;
    intCoalCfg.threshold = 1;
    intCoalCfg.timeout = 0;

    status = sdHostDriver->cQSetIntCoalescingConfig(sdHost, &intCoalCfg);
    CHECK_STATUS(status);

    sdHostDriver->cQGetIntCoalescingTimeoutBase(sdHost, &clockFreqKHz);
    SubPrint("Configure interrupt coalescing timeout to %lu KHz\n",
             (unsigned long)clockFreqKHz * intCoalCfg.timeout);

    isrCounter = 0;
    status = CQWriteReadCompareSplit2(withInt, 1);
    CHECK_STATUS(status);

    if (isrCounter != 8) {
        SubPrint("Number of occurred interrupts are different than suspected %d != 8\n\n",
                 isrCounter);
        return 1;
    }

    intCoalCfg.threshold = 4;
    intCoalCfg.timeout = 0;
    status = sdHostDriver->cQSetIntCoalescingConfig(sdHost, &intCoalCfg);
    status = sdHostDriver->cQGetIntCoalescingConfig(sdHost, &intCoalCfg);
    status = sdHostDriver->cQSetIntCoalescingConfig(sdHost, &intCoalCfg);
    CHECK_STATUS(status);

    isrCounter = 0;
    status = CQWriteReadCompareSplit2(withInt, 1);
    CHECK_STATUS(status);

    intCoalCfg.enable = 0;
    sdHostDriver->cQSetIntCoalescingConfig(sdHost, &intCoalCfg);

    if (isrCounter != 2) {
        SubPrint("Number of occurred interrupts are different than suspected %d != 2\n\n",
                 isrCounter);
        return 1;
    }

    return status;
}

static uint32_t readPhyReg(uint32_t address)
{
    uint32_t value = 0;
    CSDD_SDIO_Host* sdPd = sdHost;
    CPS_REG_WRITE(&sdPd->RegOffset->HRS.HRS04, address);
    value = CPS_REG_READ(&sdPd->RegOffset->HRS.HRS05);
    return value;
}

static void writePhyReg(uint32_t address, uint32_t value)
{
    CSDD_SDIO_Host* sdPd = sdHost;
    CPS_REG_WRITE(&sdPd->RegOffset->HRS.HRS04, address);
    CPS_REG_WRITE(&sdPd->RegOffset->HRS.HRS05, value);
    uint32_t readValue = readPhyReg(address);
    if (readValue != value) {
        SubPrint("Error, CCP: Writing failed!: address: 0x%x, value : 0x%x, readValue: 0x%x\n",
                 address, value, readValue);
    }
}

static uint32_t ccpInitDriver(void)
{

    CCP_SysReq sysReq = { 0 };
    CCP_Config cfg = { 0 };

    cfg.readPhyReg = readPhyReg;
    cfg.writePhyReg = writePhyReg;

    ccpDriver = CCP_GetInstance();
    ccpDriverExt = CCPEXT_GetInstance();

    if ((ccpDriver == NULL) && (ccpDriverExt == NULL)) {
        SubPrint("Error: CCP driver get instance failed\n");
        return CDN_EINVAL;
    }

    if (ccpDriver->probe(&cfg, &sysReq)) {
        SubPrint("Error: CCPR driver probe failed!\n");
        return CDN_EINVAL;
    }

    ccpPD = malloc(sysReq.privDataSize);

    if (ccpPD == NULL) {
        SubPrint("Error. CCP driver - failed to allocate driver private data.\n");
        return CDN_EINVAL;
    }
    SubPrint("CCP DriverAllocated %d bytes for private data at: %p\n",
             sysReq.privDataSize, ccpPD);
    if (ccpDriver->init(ccpPD, &cfg) != CDN_EOK) {
        SubPrint("Error: CCP driver init failed\n");
        return CDN_EINVAL;
    }
    SubPrint("Driver initialized.\n");
    return CDN_EOK;
}

uint8_t UHSTest(uint8_t slotIndex, uint32_t sectorNumber)
{
    uint8_t status;
    int i;
    uint32_t clockFreqsKHz[] = {25000, 50000, 100000, 208000};
    CSDD_SpeedMode cardModes[] = {CSDD_ACCESS_MODE_SDR12,
                                  CSDD_ACCESS_MODE_SDR25,
                                  CSDD_ACCESS_MODE_SDR50,
                                  CSDD_ACCESS_MODE_SDR104};

    char *modesText[] = {"SDR12", "SDR25", "SDR50", "SDR104"};
    uint32_t freq = clockFreqsKHz[0];
    unsigned char size = sizeof(freq);

    status = sdHostDriver->configure(sdHost, slotIndex,
                                     CSDD_CONFIG_SET_CLK, &freq, &size);

    for (i = 0; i < sizeof(clockFreqsKHz) / sizeof(int); i++) {
        uint32_t freq = clockFreqsKHz[i];
        unsigned char size = sizeof(freq);
        uint32_t datTimeoutUs;
        /* set card mode */

        status = sdHostDriver->configureAccessMode(sdHost, slotIndex,
                                                   cardModes[i]);
        if (status == ENOTSUP) {
            SubPrint("\t %s mode is not supported by a card\n", modesText[i]);
            continue;
        }
        CHECK_STATUS(status);
        SubPrint("\tChange access mode to %s \n", modesText[i]);
        if (cardModes[i] == CSDD_ACCESS_MODE_SDR12)
        {
            status = dfisdr12Init();
            CHECK_STATUS(status);

        }

        if (cardModes[i] == CSDD_ACCESS_MODE_SDR25)
        {
            status = dfisdr25Init();
            CHECK_STATUS(status);

        }
        if (cardModes[i] == CSDD_ACCESS_MODE_SDR104)
        {
            status = dfisdr104Init();
            CHECK_STATUS(status);

        }
        if (cardModes[i] == CSDD_ACCESS_MODE_SDR50)
        {
            status = dfisdr50Init();
            CHECK_STATUS(status);
        }

        /* set new clock frequency */
        status = sdHostDriver->configure(sdHost, slotIndex,
                                         CSDD_CONFIG_SET_CLK, &freq, &size);
        CHECK_STATUS(status);
        SubPrint("\tChange clock frequency to %dKHz \n", freq);

        if (cardModes[i] == CSDD_ACCESS_MODE_SDR104) {
            datTimeoutUs = 100;
            size = sizeof(datTimeoutUs);
            status = sdHostDriver->configure(sdHost, slotIndex,
                                             CSDD_CONFIG_SET_DAT_TIMEOUT,
                                             &datTimeoutUs, &size);
            CHECK_STATUS(status);
            status = sdHostDriver->tuning(sdHost, slotIndex);
            CHECK_STATUS(status);
            datTimeoutUs = 500000;
            status = sdHostDriver->configure(sdHost, slotIndex,
                                             CSDD_CONFIG_SET_DAT_TIMEOUT,
                                             &datTimeoutUs, &size);
            CHECK_STATUS(status);
        }

        status = WriteReadCompare(slotIndex, sectorNumber, 2048);
        CHECK_STATUS(status);

        sectorNumber += 4;
    }

    return 0;
}

uint8_t SubcommandTest(uint8_t slotIndex, uint32_t sectorNumber,
                       uint32_t DataSize, uint32_t Count)
{
    uint8_t status;
    uint8_t *wbuf = writeBuffer;
    uint16_t rca;
    CSDD_Request psubRequest = {0};

    CSDD_Request pRequest = {
        .pCmd[0].command = 25,
        .pCmd[0].argument = sectorNumber,
        .pCmd[0].requestFlags.commandType = CSDD_CMD_TYPE_NORMAL,
        .pCmd[0].requestFlags.responseType = CSDD_RESPONSE_R1,
        .pCmd[0].requestFlags.hwResponseCheck = 1,
        .pCmd[0].blockCount = 5,
        .pCmd[0].blockLen = 512,
        .pCmd[0].pDataBuffer = wbuf,
        .pCmd[0].requestFlags.dataPresent = 1,
        .pCmd[0].requestFlags.dataTransferDirection = CSDD_TRANSFER_WRITE,
        .pCmd[0].requestFlags.autoCMD12Enable = 1,
        .pCmd[0].requestFlags.autoCMD23Enable = 0,
        .pCmd[0].requestFlags.appCmd = 0,
        .busyCheckFlags = 0,
        .requestType = (uint8_t)CSDD_REQUEST_TYPE_SD,
        .pCmd[0].subBuffersCount = 0,
        .pCmd[0].requestFlags.isInfinite = 0,
        .commandCategory = CSDD_CMD_CAT_NORMAL,
        .cmdCount = 1
    };

    psubRequest = (CSDD_Request) {.pCmd[0].command = 13,
        .pCmd[0].argument = (uint32_t)rca << 16,
        .pCmd[0].requestFlags.commandType = CSDD_CMD_TYPE_NORMAL,
        .pCmd[0].requestFlags.responseType = CSDD_RESPONSE_R1,
        .pCmd[0].requestFlags.hwResponseCheck = 0,
        .pCmd[0].pDataBuffer = NULL,
        .pCmd[0].requestFlags.dataPresent = 0,
        .pCmd[0].requestFlags.appCmd = 0,
        .busyCheckFlags = 0,
        .requestType = (uint8_t)CSDD_REQUEST_TYPE_SD,
        .pCmd[0].requestFlags.isInfinite = 0,
        .pCmd[0].blockCount = 0,
        .pCmd[0].blockLen = 0,
        .status = 1,
        .commandCategory = CSDD_CMD_CAT_NORMAL,
        .cmdCount = 1
    };

    if (sdHost->hostCtrlVer < 6) {
        printf("Subcommand is not supported for host %d\n",
               sdHost->hostCtrlVer);
        return CDN_ENOTSUP;
    }

    /*  execute command to Read */
    sdHostDriver->execCardCommand(sdHost, slotIndex, &pRequest);

    status = sdHostDriver->getRca(sdHost, 0, &rca);
    if (status) {
        printf("error cannot get RCA address\n");
        return 1;
    }
    /* subcommand - CMD13 */

    sdHostDriver->execCardCommand(sdHost, slotIndex, &psubRequest);
    sdHostDriver->waitForRequest(pRequest.pSdioHost, &pRequest);
    /* check subcommand status
     * ADMA3 won't support subcommand default */
    sdHostDriver->waitForRequest(pRequest.pSdioHost, &pRequest);
    /* check main command status */
    if (pRequest.status) {
        return 1;
    }

    return 0;
}

void *descPtr;
int CSSD_RunFunctionalTests(void)
{
    uint8_t status;
    uint8_t slotIndex = 0;
    unsigned int sectorNumber = 0;
    CSDD_SysReq sysReq;
    CSDD_InterfaceType intType;
    CSDD_DeviceState deviceState;
    uint8_t withInt = 0;

    memset(&sysReq, 0, sizeof(CSDD_SysReq));
    memset(&config, 0, sizeof(CSDD_Config));
    memset(&cb, 0, sizeof(CSDD_Callbacks));

    writeBuffer = (unsigned char*)malloc(600 * 1024);
    readBuffer = (unsigned char*)malloc(0x00004000);
    if (writeBuffer == 0 || readBuffer == 0) {
        SubPrint("memory allocation problem\r\n");
        while (1) {; }
    }
    sdHostDriver = CSDD_GetInstance();

    sdHostDriver->probe(SD_REG_BASE, &sysReq);
    sdHost = malloc(sysReq.pDataSize);
    memset(sdHost, 0, sysReq.pDataSize);

    config.descLogAddress = (uint32_t*)malloc(sysReq.descSize);
    config.descPhyAddress = config.descLogAddress;
    config.idDescLogAddress = (uint32_t*)malloc(sysReq.idDescSize);
    config.idDescPhyAddress = config.idDescLogAddress;
    config.regBase = SD_REG_BASE;

    if (sizeof(void*) > 4) {
        config.dma64BitEn = 1;
    }
    else{
        config.dma64BitEn = 0;
    }

    cb.setTuneValCallback = &setTuneVal;

    status = sdHostDriver->init(sdHost, &config, &cb);
    CHECK_STATUS(status);
    /* Combo PHY Driver init and config PHY*/

    if (sdHost->hostCtrlVer >= 6) {
        status = ccpInitDriver();
        CHECK_STATUS(status);

        status = dfiInit();
        CHECK_STATUS(status);
    }

    Clearbuf(readBuffer, 0x4000, 0xDEADC0DE);
    Fillbuf(writeBuffer, 100 * 1024, 5);

    /* load memory card driver, it is necessary to use this driver */
    sdHostDriver->memoryCardLoadDriver(sdHost);

    CsddInterruptInit(sdHost, &Isr);
    withInt = 1;
    sdHostDriver->start(sdHost);

    /* Check if there is a card in a slot
     * if card is present then it will be initialized in this function */
    status = sdHostDriver->checkSlots(sdHost);

    CHECK_STATUS(status);

    SubPrint("\n");
    sdHostDriver->getInterfaceType(sdHost, slotIndex, &intType);
    sdHostDriver->getDeviceState(sdHost, slotIndex, &deviceState);

    if (!deviceState.inserted) {
        printf("There is no card in slot\n");
        return 1;
    }
    if (intType == CSDD_INTERFACE_TYPE_SD) {
        unsigned char busWidth = CSDD_BUS_WIDTH_4;
        unsigned char size = sizeof(busWidth);
        uint32_t freq = 10000;

        /* change bus width */
        status = sdHostDriver->configure(sdHost, slotIndex,
                                         CSDD_CONFIG_SET_BUS_WIDTH,
                                         &busWidth, &size);

        size = sizeof(freq);
        status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                         &freq, &size);
        CHECK_STATUS(status);
    }

    if (sdHost->hostCtrlVer == 4) {
        /* since we are using combo PHY not required */
        if (deviceState.deviceType == CSDD_CARD_TYPE_SDMEM) {
            testResult("PhyTrainingUhs", PhyTrainingUhs(slotIndex));
        }

        if (deviceState.deviceType == CSDD_CARD_TYPE_MMC) {
            testResult("PhyTrainingMmc", PhyTrainingMmc(slotIndex));
        }
    }

    DbgMsgSetLvl(DBG_CRIT);

    testResult("SimpleTest", SimpleTest(slotIndex, sectorNumber));
    sectorNumber += 16;
    testResult("SubcommandTest", SubcommandTest(slotIndex, sectorNumber, 1024,
                                                4));
    sectorNumber += 16;
    testResult("InfiniteReadTest", InfiniteReadTest(slotIndex, sectorNumber,
                                                    1024, 4));
    sectorNumber += 16;
    testResult("InfiniteWriteTest", InfiniteWriteTest(slotIndex, sectorNumber,
                                                      1024, 4));
    sectorNumber += 16;
    testResult("SingleSectorTest", SingleSectorTest(slotIndex, sectorNumber));
    testResult("DMATest", DMATest(slotIndex, sectorNumber));
    sectorNumber += 16;
    testResult("ADMA3Test", ADMA3Test(slotIndex, sectorNumber));
    sectorNumber += 16;
    if (USE_AUTO_CMD) {
        testResult("NonBlockingTest", NonBlockingTest(slotIndex, sectorNumber));
    }

    if (intType == CSDD_INTERFACE_TYPE_SD) {
        testResult("LowClockFreqTest", LowClockFreqTest(slotIndex,
                                                        sectorNumber));

        if (deviceState.uhsSupported) {
            testResult("UHSTest", UHSTest(slotIndex, sectorNumber));
        }
        else {
            testResult("HSTest", HSTest(slotIndex, sectorNumber));
        }

        if (deviceState.deviceType == CSDD_CARD_TYPE_SDMEM) {
            testResult("BusWidthTest", BusWidthTest(slotIndex, sectorNumber,
                                                    busWidthArray, 2));
            sectorNumber += 16;
        }
    }

    if (deviceState.deviceType == CSDD_CARD_TYPE_MMC) {
        descPtr = malloc(8192);

        if (!descPtr) {
            SubPrint("Descriptors allocation failed\r\n");
            return -1;
        }

        testResult("BusWidthTest", BusWidthTest(slotIndex, sectorNumber,
                                                busWidthArray, 3));
        sectorNumber += 16;
        testResult("MmchighSpeedTest", MmchighSpeedTest(slotIndex,
                                                        sectorNumber));
        sectorNumber += 16;

        status = CQEnable();
        if (status == 0) {
            testResult("CQ Direct command test", CQExecDirectCommand(withInt));
            testResult("CQ Data transfer test", CQWriteReadCompare(withInt));

            sdHostDriver->stop(sdHost);

            testResult("CQ Direct command pooling mode test",
                       CQExecDirectCommand(0));
            testResult("CQ Data transfer pooling mode test",
                       CQWriteReadCompare(0));

            if (withInt == 1) {
                unsigned char busWidth = CSDD_BUS_WIDTH_4;
                unsigned char size = sizeof(busWidth);
                uint32_t freq = 10000;

                sdHostDriver->start(sdHost);

                testResult("CQ Split data across queues test",
                  CQWriteReadCompareSplit2(withInt, 0));
                  testResult("CQ Split data across descriptors test",
                  CQWriteReadCompareSplited(withInt, descPtr, 2048));

                /* Reset card to set slower transfer mode.
                 * To make sure that we do not miss an interrupt.*/
                status = ReinitHostAndDevice();
                CHECK_STATUS(status);
                status = CQEnable();
                CHECK_STATUS(status);

                status = sdHostDriver->configure(sdHost, slotIndex,
                                                 CSDD_CONFIG_SET_BUS_WIDTH,
                                                 &busWidth, &size);

                size = sizeof(freq);
                status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                                 &freq, &size);
                CHECK_STATUS(status);

                testResult("CQ Interrupt coalescing test",
                           CQInterruptCoalescingTest(withInt));
            }
            CQDisable();
        }
    }

    return 0;
}

TestData testData;

void testResult(char* testName, uint32_t errorCode){
    if(errorCode == CDN_EOK)
        TESTING_TEST_PASSED(testName, testData);
    else if (errorCode == CDN_ENOTSUP)
        TESTING_TEST_NOTSUP(testName, testData);
    else
        TESTING_TEST_FAILED(testName, testData);
}

int main()
{
    int result = -1;
    char testGroupName[100] = "Functional tests";

#ifdef EMMC
    snprintf(testGroupName, 100, "Functional tests - eMMC");
#endif
#ifdef SD
    snprintf(testGroupName, 100, "Functional tests - SD");
#endif

    DbgMsgSetLvl(DBG_FYI);
    DbgMsgEnableModule(DBG_GEN_MSG);
    csp_test_start();

    testData.groupName = testGroupName;
    testData.groupIndex = 0;
    START_TESTING(testData);

    CSSD_RunFunctionalTests();

    TESTING_SUMMARY(testData);

    if (testData.calls)
        result = testData.failed;

    csp_test_complete(result);

    return result;
}
