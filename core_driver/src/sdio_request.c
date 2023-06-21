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
 * sdio_request.c
 * SD Host controller driver - Request creator module
 *****************************************************************************/

#include "sdio_request.h"
#include "csdd_structs_if.h"

void SDIO_REQ_INIT_CMD(CSDD_Request* req, const SD_CsddRequesParams* params)
{
    req->pCmd->command = params->cmd;
    req->pCmd->argument = params->arg;
    req->pCmd->requestFlags.commandType = params->cmdType;
    req->pCmd->requestFlags.responseType = params->respType;
    req->pCmd->requestFlags.hwResponseCheck = params->hwRespCheck;
    req->pCmd->pDataBuffer = NULL;
    req->pCmd->requestFlags.dataPresent = 0;
    req->pCmd->requestFlags.appCmd = 0;
    req->busyCheckFlags = 0;
    req->requestType = (uint8_t)CSDD_REQUEST_TYPE_SD;
    req->pCmd->requestFlags.isInfinite = 0;
    req->pCmd->blockCount = 0;
    req->pCmd->blockLen = 0;
    req->status = 1;
    req->commandCategory = CSDD_CMD_CAT_NORMAL;
    req->cmdCount = 1;
}

void SDIO_REQ_INIT_ACMD(CSDD_Request* req, const SD_CsddRequesParams* params)
{
    req->pCmd->command = params->cmd;
    req->pCmd->argument = params->arg;
    req->pCmd->requestFlags.commandType = params->cmdType;
    req->pCmd->requestFlags.responseType = params->respType;
    req->pCmd->requestFlags.hwResponseCheck = params->hwRespCheck;
    req->pCmd->pDataBuffer = NULL;
    req->pCmd->requestFlags.dataPresent = 0;
    req->pCmd->requestFlags.appCmd = 1;
    req->busyCheckFlags = 0;
    req->requestType = (uint8_t)CSDD_REQUEST_TYPE_SD;
    req->pCmd->requestFlags.isInfinite = 0;
    req->pCmd->blockCount = 0;
    req->pCmd->blockLen = 0;
    req->commandCategory = CSDD_CMD_CAT_NORMAL;
    req->cmdCount = 1;
}

void SDIO_REQ_INIT_CMD_WITH_DATA(CSDD_Request* req, const SD_CsddRequesParams* params,
                                 const SD_CsddRequesParamsExt* paramsExt)
{
    req->pCmd->command = params->cmd;
    req->pCmd->argument = params->arg;
    req->pCmd->requestFlags.commandType = params->cmdType;
    req->pCmd->requestFlags.responseType = params->respType;
    req->pCmd->requestFlags.hwResponseCheck = params->hwRespCheck;
    req->pCmd->blockCount = paramsExt->blkCount;
    req->pCmd->blockLen = paramsExt->blkLen;
    req->pCmd->pDataBuffer = paramsExt->buf;
    req->pCmd->requestFlags.dataPresent = 1;
    req->pCmd->requestFlags.dataTransferDirection = paramsExt->dir;
    req->pCmd->requestFlags.autoCMD12Enable = paramsExt->auto12;
    req->pCmd->requestFlags.autoCMD23Enable = paramsExt->auto23;
    req->pCmd->requestFlags.appCmd = paramsExt->appCmd;
    req->busyCheckFlags = 0;
    req->requestType = (uint8_t)CSDD_REQUEST_TYPE_SD;
    req->pCmd->subBuffersCount = paramsExt->subBuffersCount;
    req->pCmd->requestFlags.isInfinite = 0;
    req->commandCategory = CSDD_CMD_CAT_NORMAL;
    req->cmdCount = 1;
}

void SDIO_REQ_INIT_CMD_DATA_INF_S(CSDD_Request* req, const SD_CsddRequesParams* params,
                                       const SD_CsddRequesParamsExt* paramsExt)
{
    req->pCmd->command = params->cmd;
    req->pCmd->argument = params->arg;
    req->pCmd->requestFlags.commandType = params->cmdType;
    req->pCmd->requestFlags.responseType = params->respType;
    req->pCmd->requestFlags.hwResponseCheck = params->hwRespCheck;
    req->pCmd->blockCount = paramsExt->blkCount;
    req->pCmd->blockLen = paramsExt->blkLen;
    req->pCmd->pDataBuffer = paramsExt->buf;
    req->pCmd->requestFlags.dataPresent = 1;
    req->pCmd->requestFlags.dataTransferDirection = paramsExt->dir;
    req->pCmd->requestFlags.autoCMD12Enable = 0;
    req->pCmd->requestFlags.autoCMD23Enable = 0;
    req->pCmd->requestFlags.appCmd = 0;
    req->busyCheckFlags = 0;
    req->requestType = (uint8_t)CSDD_REQUEST_TYPE_SD;
    req->pCmd->subBuffersCount = paramsExt->subBuffersCount;
    req->pCmd->requestFlags.isInfinite = 1;
    req->infiniteStatus = SDIOHOST_REQUEST_ISTATUS_FIRST;
    req->commandCategory = CSDD_CMD_CAT_NORMAL;
    req->cmdCount = 1;
}

void SDIO_REQ_INIT_CMD_DATA_INF_N(CSDD_Request* req, void* buf,
                                       uint32_t blk_count,  uint16_t blk_len,
                                       CSDD_TransferDirection dir)
{
    req->pCmd->blockCount = blk_count;
    req->pCmd->blockLen = blk_len;
    req->pCmd->pDataBuffer = buf;
    req->pCmd->requestFlags.dataPresent = 1;
    req->pCmd->requestFlags.dataTransferDirection = dir;
    req->pCmd->requestFlags.appCmd = 0;
    req->requestType = (uint8_t)CSDD_REQUEST_TYPE_SD;
    req->pCmd->subBuffersCount = 0;
    req->pCmd->requestFlags.isInfinite = 1;
    req->infiniteStatus = SDIOHOST_REQUEST_ISTATUS_NEXT;
    req->cmdCount = 1;
}
void SDIO_REQ_INIT_CMD_DATA_MULTI(CSDD_Request* req, uint8_t cmdCount)
{
    req->cmdCount = cmdCount;
    req->busyCheckFlags = 0;
    req->commandCategory = CSDD_CMD_CAT_NORMAL;
    req->requestType = (uint8_t)CSDD_REQUEST_TYPE_SD;
}

