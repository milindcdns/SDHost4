/* parasoft-begin-suppress MISRA2012-RULE-1_1_a_c99-2 "C99 - limits, DRV-4754" parasoft-begin-suppress MISRA2012-RULE-1_1_a_c90-2 "C90 - limits, DRV-4754" */
/* parasoft-begin-suppress item MISRA2012-DIR-4_8 "Consider hiding implementation of structure, DRV-3667" */

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
 * THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT
 *
 **********************************************************************/

#ifndef REG_CQRS_H_
#define REG_CQRS_H_

#include <cps_drv.h>

#include "sd4hc_cqrs_macros.h"

typedef struct __attribute__((packed)) SD4HC_Cqrs_s {
        volatile uint32_t CQRS00;
        volatile uint32_t CQRS01;
        volatile uint32_t CQRS02;
        volatile uint32_t CQRS03;
        volatile uint32_t CQRS04;
        volatile uint32_t CQRS05;
        volatile uint32_t CQRS06;
        volatile uint32_t CQRS07;
        volatile uint32_t CQRS08;
        volatile uint32_t CQRS09;
        volatile uint32_t CQRS10;
        volatile uint32_t CQRS11;
        volatile uint32_t CQRS12;
        volatile uint32_t CQRS13;
        volatile uint32_t CQRS14;
        volatile char pad__0[0x4U];
        volatile uint32_t CQRS16;
        volatile uint32_t CQRS17;
        volatile uint32_t CQRS18;
        volatile char pad__1[0x4U];
        volatile uint32_t CQRS20;
        volatile uint32_t CQRS21;
        volatile uint32_t CQRS22;
        volatile uint32_t CQRS23;
} SD4HC_Cqrs;

#endif /* REG_CQRS_H_ */

/* parasoft-end-suppress MISRA2012-RULE-1_1_a_c99-2 "C99 - limits, DRV-4754" */
/* parasoft-end-suppress MISRA2012-RULE-1_1_a_c90-2 "C90 - limits, DRV-4754" */
/* parasoft-end-suppress MISRA2012-DIR-4_8 "Consider hiding implementation of structure, DRV-3667" */

