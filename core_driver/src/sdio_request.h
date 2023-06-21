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
 * sdio_request.h
 * SD Host controller driver - Request creator module
 *****************************************************************************/

#ifndef SDIO_REQUEST_H
#define SDIO_REQUEST_H

#include "sdio_types.h"

#define SDIOHOST_REQUEST_ISTATUS_FIRST      0U
#define SDIOHOST_REQUEST_ISTATUS_NEXT       1U

typedef struct {
    uint8_t cmd;
    uint32_t arg;
    CSDD_CmdType cmdType;
    CSDD_ResponseType respType;
    uint8_t hwRespCheck;
} SD_CsddRequesParams;

typedef struct {
    void* buf;
    uint32_t blkCount;
    uint16_t blkLen;
    uint8_t auto12;
    uint8_t auto23;
    CSDD_TransferDirection dir;
    uint8_t appCmd;
    uint8_t subBuffersCount;
} SD_CsddRequesParamsExt;

typedef struct {
    void* buf;
    uint32_t blkCount;
    uint16_t blkLen;
    CSDD_TransferDirection dir;
    uint8_t subBuffersCount;
} SD_CsddRequesParamsADMA3Ext;

void SDIO_REQ_INIT_CMD(CSDD_Request* req, const SD_CsddRequesParams* params);

void SDIO_REQ_INIT_ACMD(CSDD_Request* req, const SD_CsddRequesParams* params);

void SDIO_REQ_INIT_CMD_WITH_DATA(CSDD_Request* req, const SD_CsddRequesParams* params,
                                 const SD_CsddRequesParamsExt* paramsExt);

void SDIO_REQ_INIT_CMD_DATA_INF_S(CSDD_Request* req, const SD_CsddRequesParams* params,
                                       const SD_CsddRequesParamsExt* paramsExt);

void SDIO_REQ_INIT_CMD_DATA_INF_N(CSDD_Request* req, void* buf,
                                       uint32_t blk_count,  uint16_t blk_len,
                                       CSDD_TransferDirection dir);
void SDIO_REQ_INIT_CMD_DATA_MULTI(CSDD_Request* req, uint8_t cmdCount);

#endif
