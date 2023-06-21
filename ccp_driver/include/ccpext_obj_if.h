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


 **********************************************************************
 * WARNING: This file is auto-generated using api-generator utility.
 *          api-generator: 13.05.b3ee589
 *          Do not edit it manually.
 **********************************************************************
 * Extracted data
 **********************************************************************/
#ifndef CCPEXT_OBJ_IF_H
#define CCPEXT_OBJ_IF_H

#include "ccpext_if.h"

/** @defgroup DriverObject Driver API Object
 *  API listing for the driver. The API is contained in the object as
 *  function pointers in the object structure. As the actual functions
 *  resides in the Driver Object, the client software must first use the
 *  global GetInstance function to obtain the Driver Object Pointer.
 *  The actual APIs then can be invoked using obj->(api_name)() syntax.
 *  These functions are defined in the header file of the core driver
 *  and utilized by the API.
 *  @{
 */

/**********************************************************************
 * API methods
 **********************************************************************/

/* parasoft-begin-suppress MISRA2012-DIR-4_8-4 "Consider hiding the implementation of the structure, DRV-4932" */
typedef struct CCPEXT_OBJ_s
{
    /**
     * setter for PhyWrDeskewPdCtrl0Dqx
     * @param[in] pD Pointer to the private data initialized by init
     * @param[in] value input struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*setPhyWrDeskewPdCtrl0Dqx)(CCP_PrivateData* pD, const CCPEXT_PhyWrDeskewPdCtrl0Dqx* value);

    /**
     * getter for PhyWrDeskewPdCtrl0Dqx
     * @param[in] pD Pointer to the private data initialized by init
     * @param[out] value output struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*getPhyWrDeskewPdCtrl0Dqx)(const CCP_PrivateData* pD, CCPEXT_PhyWrDeskewPdCtrl0Dqx* value);

    /**
     * setter for PhyWrDeskewReg
     * @param[in] pD Pointer to the private data initialized by init
     * @param[in] value input struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*setPhyWrDeskewReg)(CCP_PrivateData* pD, const CCPEXT_PhyWrDeskewReg* value);

    /**
     * getter for PhyWrDeskewReg
     * @param[in] pD Pointer to the private data initialized by init
     * @param[out] value output struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*getPhyWrDeskewReg)(const CCP_PrivateData* pD, CCPEXT_PhyWrDeskewReg* value);

    /**
     * setter for PhyRdDeskewReg
     * @param[in] pD Pointer to the private data initialized by init
     * @param[in] value input struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*setPhyRdDeskewReg)(CCP_PrivateData* pD, const CCPEXT_PhyRdDeskewReg* value);

    /**
     * getter for PhyRdDeskewReg
     * @param[in] pD Pointer to the private data initialized by init
     * @param[out] value output struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*getPhyRdDeskewReg)(const CCP_PrivateData* pD, CCPEXT_PhyRdDeskewReg* value);

    /**
     * setter for PhyWrDeskewPdCtrl1Reg
     * @param[in] pD Pointer to the private data initialized by init
     * @param[in] value input struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*setPhyWrDeskewPdCtrl1Reg)(CCP_PrivateData* pD, const CCPEXT_PhyWrDeskewPdCtrl1Reg* value);

    /**
     * getter for PhyWrDeskewPdCtrl1Reg
     * @param[in] pD Pointer to the private data initialized by init
     * @param[out] value output struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*getPhyWrDeskewPdCtrl1Reg)(const CCP_PrivateData* pD, CCPEXT_PhyWrDeskewPdCtrl1Reg* value);

    /**
     * setter for PhyCtrlLowFreqSel
     * @param[in] pD Pointer to the private data initialized by init
     * @param[in] value input struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*setPhyCtrlLowFreqSel)(CCP_PrivateData* pD, const CCPEXT_PhyCtrlLowFreqSel* value);

    /**
     * getter for PhyCtrlLowFreqSel
     * @param[in] pD Pointer to the private data initialized by init
     * @param[out] value output struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*getPhyCtrlLowFreqSel)(const CCP_PrivateData* pD, CCPEXT_PhyCtrlLowFreqSel* value);

    /**
     * setter for PhyWrRdDeskewCmdExt
     * @param[in] pD Pointer to the private data initialized by init
     * @param[in] value input struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*setPhyWrRdDeskewCmdExt)(CCP_PrivateData* pD, const CCPEXT_PhyWrRdDeskewCmdExt* value);

    /**
     * getter for PhyWrRdDeskewCmdExt
     * @param[in] pD Pointer to the private data initialized by init
     * @param[out] value output struct
     * @return CDN_EOK on success
     * @return CDN_EINVAL if any pointer parameters are NULL
     */
    uint32_t (*getPhyWrRdDeskewCmdExt)(const CCP_PrivateData* pD, CCPEXT_PhyWrRdDeskewCmdExt* value);

} CCPEXT_OBJ;
/* parasoft-end-suppress MISRA2012-DIR-4_8-4 */

/* parasoft-begin-suppress METRICS-36-3 "A function should not be called from more than 5 different functions, DRV-3666" */
/**
 * In order to access the CCPEXT APIs, the upper layer software must call
 * this global function to obtain the pointer to the driver object.
 * @return CCPEXT_OBJ* Driver Object Pointer
 */
extern CCPEXT_OBJ *CCPEXT_GetInstance(void);

/**
 *  @}
 */
/* parasoft-end-suppress METRICS-36-3 */


#endif	/* CCPEXT_OBJ_IF_H */
