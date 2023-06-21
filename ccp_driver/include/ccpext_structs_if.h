/* parasoft suppress item  MISRA2012-DIR-4_8 "Consider hiding implementation of structure, DRV-4932" */
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
#ifndef CCPEXT_STRUCTS_IF_H
#define CCPEXT_STRUCTS_IF_H

#include "cdn_stdtypes.h"
#include "ccpext_if.h"

/** @defgroup DataStructure Dynamic Data Structures
 *  This section defines the data structures used by the driver to provide
 *  hardware information, modification and dynamic operation of the driver.
 *  These data structures are defined in the header file of the core driver
 *  and utilized by the API.
 *  @{
 */

/**********************************************************************
 * Structures and unions
 **********************************************************************/
/** This struct holds the values of delay of each DQ bit on the write path. */
struct CCPEXT_PhyWrDeskewReg_s
{
    /** Deskew delay for DQ bit 0. */
    uint8_t wrDq0DeskwDelay;
    /** Deskew delay for DQ bit 1. */
    uint8_t wrDq1DeskwDelay;
    /** Deskew delay for DQ bit 2. */
    uint8_t wrDq2DeskwDelay;
    /** Deskew delay for DQ bit 3. */
    uint8_t wrDq3DeskwDelay;
    /** Deskew delay for DQ bit 4. */
    uint8_t wrDq4DeskwDelay;
    /** Deskew delay for DQ bit 5. */
    uint8_t wrDq5DeskwDelay;
    /** Deskew delay for DQ bit 6. */
    uint8_t wrDq6DeskwDelay;
    /** Deskew delay for DQ bit 7. */
    uint8_t wrDq7DeskwDelay;
};

/** This struct holds the values of delay of each DQ bit on the read path. */
struct CCPEXT_PhyRdDeskewReg_s
{
    /** Deskew delay for DQ bit 0. */
    uint8_t rdDq0DeskwDelay;
    /** Deskew delay for DQ bit 1. */
    uint8_t rdDq1DeskwDelay;
    /** Deskew delay for DQ bit 2. */
    uint8_t rdDq2DeskwDelay;
    /** Deskew delay for DQ bit 3. */
    uint8_t rdDq3DeskwDelay;
    /** Deskew delay for DQ bit 4. */
    uint8_t rdDq4DeskwDelay;
    /** Deskew delay for DQ bit 5. */
    uint8_t rdDq5DeskwDelay;
    /** Deskew delay for DQ bit 6. */
    uint8_t rdDq6DeskwDelay;
    /** Deskew delay for DQ bit 7. */
    uint8_t rdDq7DeskwDelay;
};

/** This struct holds the values of phase detect block for each DQ DQ bit on the write path. */
struct CCPEXT_PhyWrDeskewPdCtrl0Dqx_s
{
    /** DQ3 SW Phase Bypass. */
    uint8_t dq3SwDqPhaseBypass;
    /** DQ3 EN SW half cycle. */
    uint8_t dq3EnSwHalfCycle;
    /** DQ3 SW half cycle shift. */
    uint8_t dq3SwHalfCycleShift;
    /** DQ3 phase detect sel. */
    uint8_t dq3PhaseDetectSel;
    /** DQ2 SW Phase Bypass. */
    uint8_t dq2SwDqPhaseBypass;
    /** DQ2 EN SW half cycle. */
    uint8_t dq2EnSwHalfCycle;
    /** DQ2 SW half cycle shift. */
    uint8_t dq2SwHalfCycleShift;
    /** DQ2 phase detect sel. */
    uint8_t dq2PhaseDetectSel;
    /** DQ1 SW Phase Bypass. */
    uint8_t dq1SwDqPhaseBypass;
    /** DQ1 EN SW half cycle. */
    uint8_t dq1EnSwHalfCycle;
    /** DQ1 SW half cycle shift. */
    uint8_t dq1SwHalfCycleShift;
    /** DQ1 phase detect sel. */
    uint8_t dq1PhaseDetectSel;
};

/** This struct holds the values of phase detect block for each DQ DQ bit on the write path. */
struct CCPEXT_PhyWrDeskewPdCtrl1Reg_s
{
    /** DQ7 SW Phase Bypass. */
    uint8_t dq7SwDqPhaseBypass;
    /** DQ7 EN SW half cycle. */
    uint8_t dq7EnSwHalfCycle;
    /** DQ7 SW half cycle shift. */
    uint8_t dq7SwHalfCycleShift;
    /** DQ7 phase detect sel. */
    uint8_t dq7PhaseDetectSel;
    /** DQ6 SW Phase Bypass. */
    uint8_t dq6SwDqPhaseBypass;
    /** DQ6 EN SW half cycle. */
    uint8_t dq6EnSwHalfCycle;
    /** DQ6 SW half cycle shift. */
    uint8_t dq6SwHalfCycleShift;
    /** DQ6 phase detect sel. */
    uint8_t dq6PhaseDetectSel;
    /** DQ5 SW Phase Bypass. */
    uint8_t dq5SwDqPhaseBypass;
    /** DQ5 EN SW half cycle. */
    uint8_t dq5EnSwHalfCycle;
    /** DQ5 SW half cycle shift. */
    uint8_t dq5SwHalfCycleShift;
    /** DQ5 phase detect sel. */
    uint8_t dq5PhaseDetectSel;
    /** DQ4 SW Phase Bypass. */
    uint8_t dq4SwDqPhaseBypass;
    /** DQ4 EN SW half cycle. */
    uint8_t dq4EnSwHalfCycle;
    /** DQ4 SW half cycle shift. */
    uint8_t dq4SwHalfCycleShift;
    /** DQ4 phase detect sel. */
    uint8_t dq4PhaseDetectSel;
};

/** This register handles the global control settings for the PHY. Please make sure all the reserved fields are set to 0. */
struct CCPEXT_PhyCtrlLowFreqSel_s
{
    /** If this field is set high the DFI interface is synchronous to the falling edge of the clock. */
    uint8_t lowFreqSel;
};

/**
 * This structs holds the values of delay of CMD bit on the write and read path path as well as the values of phase detect
 * block for CMD bit on the write path.
*/
struct CCPEXT_PhyWrRdDeskewCmdExt_s
{
    /** Deskew delay (wrCmd). */
    uint8_t wrCmdDeskewDelay;
    /** Deskew delay (rdCmd). */
    uint8_t rdCmdDeskewDelay;
};

/**
 *  @}
 */

#endif	/* CCPEXT_STRUCTS_IF_H */
