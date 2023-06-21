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
 * reference code showing how to use eMMC boot operation
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _HAVE_DBG_LOG_INT_ 1
#include <cdn_log.h>
#include <test_harness_log.h>
#include <map_system_memory.h>
#include <irq.h>
#include <csp.h>
#include <common.h>

#include <csdd_if.h>
#include <csdd_obj_if.h>
#include <csdd_structs_if.h>
#include <sdio_dfi.h>


#define SubPrint printf

CSDD_Config config;
CSDD_Callbacks cb;

CSDD_OBJ *sdHostDriver;
CSDD_SDIO_Host *sdHost;

CCP_OBJ *ccpDriver;
CCPEXT_OBJ *ccpDriverExt;
CCP_PrivateData *ccpPD;


uint8_t setTuneVal(const CSDD_SDIO_Host* pd, uint8_t tune_val)
{
    printf("***********callback*************\n");
    return dfihs200Init(tune_val);
}


void Isr(void* pD)
{
    CSDD_OBJ *sdHostDriver;
    bool handled;

    sdHostDriver = CSDD_GetInstance();
    sdHostDriver->isr(pD, &handled);
}

char * testGroupName = "Functional tests - eMMC";
TestData testData;

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


void testResult(char* testName, uint32_t errorCode)
{
    if(errorCode == CDN_EOK)
        TESTING_TEST_PASSED(testName, testData);
    else if (errorCode == CDN_ENOTSUP)
        TESTING_TEST_NOTSUP(testName, testData);
    else
        TESTING_TEST_FAILED(testName, testData);
}


int mmcBootTest(uint8_t slotIndex, uint32_t sectorNumber,
                CSDD_OBJ *sdHostDriver, CSDD_SDIO_Host *sdHost)
{
    uint8_t status;
    uint32_t dataSize = 512;
    uint32_t bootSize;
    CSDD_DeviceState deviceState;

    sdHostDriver->getDeviceState(sdHost, slotIndex, &deviceState);

    if (deviceState.deviceType != CSDD_CARD_TYPE_MMC) {
        printf("Test is not supported for devices different than MMC\n");
        return CDN_ENOTSUP;
    }

    Clearbuf(readBuffer, dataSize, 0xDEADC0DE);
    Fillbuf(writeBuffer, dataSize, 3);

    status = sdHostDriver->mmcGetParitionBootSize(sdHost, slotIndex, &bootSize);
    CHECK_STATUS(status);

    printf("Boot partition Size = %d B\n", bootSize);

    if (!bootSize){
        printf("boot is not supported\n");
        return -1;
    }
    if (dataSize > bootSize)
        dataSize = bootSize;

    /* select access to partition boot 1 */
    status = sdHostDriver->mmcSetPartAccess(sdHost, slotIndex,
                                            CSDD_EMMC_ACCCESS_BOOT_1);
    CHECK_STATUS(status);

    /* program data to active partition */
    status = sdHostDriver->memoryCardDataTransfer(sdHost, slotIndex, 0,
                                                   writeBuffer,
                                                   dataSize, CSDD_TRANSFER_WRITE);
    CHECK_STATUS(status);

    /* select boot partition to boot operation 1 */
    status = sdHostDriver->mmcSetBootPartition(sdHost, slotIndex,
                                                CSDD_EMMC_BOOT_1);
    CHECK_STATUS(status);

    /* disable boot ACK (it cannot be properly handled in SW boot) */
    status = sdHostDriver->mmcSetBootAck(sdHost, slotIndex, false);
    CHECK_STATUS(status);

    /* detach device */
    status = sdHostDriver->deviceDetach(sdHost, slotIndex);
    CHECK_STATUS(status);
    printf("Device detached\n");

    /* reset host */
    status = sdHostDriver->resetHost(sdHost);
    CHECK_STATUS(status);
    printf("host reresed\n");

    /* Initialize host */
    status = sdHostDriver->init(sdHost, &config, &cb);
    CHECK_STATUS(status);
    printf("Host initialized\n");

    if (sdHost->hostCtrlVer >= 6) {
        status = dfiInit();
        CHECK_STATUS(status);
    }

    /* start interrupts */
    sdHostDriver->start(sdHost);

    /* short Initialization one slot (just enable clock and power supply) */
    status = sdHostDriver->simpleInit(sdHost, slotIndex, 400);
    CHECK_STATUS(status);

    /* execute eMMC boot to read data from boot partition */
    status = sdHostDriver->mmcExecuteBoot(sdHost, slotIndex, readBuffer,
                                          dataSize);
    printf("readBuffer %x\n", readBuffer[0]);
    CHECK_STATUS(status);

    /* compare written and read data */
    status = Comparebuf(writeBuffer, readBuffer, dataSize);
    CHECK_STATUS(status);

    return status;
}

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
        printf("memory allocation problem\r\n");
        while (1) {; }
    }
    sdHostDriver = CSDD_GetInstance();

    sdHostDriver->probe(SDHC0_REGS_APB_BASE, &sysReq);
    sdHost = malloc(sysReq.pDataSize);
    memset(sdHost, 0, sysReq.pDataSize);

    config.descLogAddress = (uint32_t*)malloc(sysReq.descSize);
    config.descPhyAddress = config.descLogAddress;
    config.idDescLogAddress = (uint32_t*)malloc(sysReq.idDescSize);
    config.idDescPhyAddress = config.idDescLogAddress;
    config.regBase = SDHC0_REGS_APB_BASE;

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

    /* load memory card driver, it is necessary to use this driver */
    sdHostDriver->memoryCardLoadDriver(sdHost);

    CsddInterruptInit(sdHost, &Isr);
    withInt = 1;
    sdHostDriver->start(sdHost);

    /* Check if there is a card in a slot
     * if card is present then it will be initialized in this function */
    status = sdHostDriver->checkSlots(sdHost);

    CHECK_STATUS(status);

    printf("\n");
    sdHostDriver->getInterfaceType(sdHost, slotIndex, &intType);
    sdHostDriver->getDeviceState(sdHost, slotIndex, &deviceState);

    if (!deviceState.inserted) {
        printf("There is no card in slot\n");
        return 1;
    }
    if (intType == CSDD_INTERFACE_TYPE_SD) {
        unsigned char busWidth = CSDD_BUS_WIDTH_8;
        unsigned char size = sizeof(busWidth);
        uint32_t freq = 10000;

        /*change bus width */
        status = sdHostDriver->configure(sdHost, slotIndex,
                                         CSDD_CONFIG_SET_BUS_WIDTH,
                                         &busWidth, &size);

        size = sizeof(freq);
        status = sdHostDriver->configure(sdHost, slotIndex, CSDD_CONFIG_SET_CLK,
                                         &freq, &size);
        CHECK_STATUS(status);
    }

    testResult("Boot test", mmcBootTest(slotIndex, sectorNumber, sdHostDriver,
                                        sdHost));

    return 0;
}

int main()
{
    int result = -1;

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
