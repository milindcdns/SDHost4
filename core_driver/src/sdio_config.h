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
 *
 ******************************************************************************
 * sdio_config.h
 * SD Host controller driver configuration
 *****************************************************************************/


#ifndef SDIO_CONFIG_H
#define SDIO_CONFIG_H

// Macros used only in preprocessor directives
//
/// SDMA support for SDIO host driver
#define SDIO_SDMA_SUPPORTED                 1
/// ADMA1 support for SDIO host driver
#define SDIO_ADMA1_SUPPORTED                0
/// ADMA2 support for SDIO host driver
#define SDIO_ADMA2_SUPPORTED                1
/// ADMA3 support for SDIO host driver from host ver6.
#define SDIO_ADMA3_SUPPORTED                1
/// procedure to handle card interrupt
#define ENABLE_CARD_INTERRUPT               0
/// change data endianity during read - dma disabled
#define CHANGE_DATA_ENDIANITY_NODMA         1
/// it enables support for IO cards
/// (it should be disabled it was not tested yet)
#define SDIO_CFG_ENABLE_IO                  0
///  disable or enable support for MMC devices
#define SDIO_CFG_ENABLE_MMC                 1
/// Version of SD Host controller
#ifndef SDIO_CFG_HOST_VER
#   define SDIO_CFG_HOST_VER                6
#endif

// Macros used as constants
/// Set debouncing period
#define DEBOUNCING_TIME                     0x300000UL
/// Commands timeout is a iteration count after which timeout error will be report
/// if a command will not execute (mainly using in WaitForValue function)
#define COMMANDS_TIMEOUT                    3000U
// system clock in Hz
#define SYTEM_CLK_KHZ                       (140000U)
/// buffer size for ADMA2 descriptors
#define MAX_DESCR_BUFF_SIZE                 (1024U * 50U)
/// buffer size for ADMA 3 command descriptors
#define MAX_COMMAND_DESCR_BUFF_SIZE         (1024U * 50U)
/// buffer size for ADMA 3 integrated descriptors
#define MAX_INTEGREATED_DESCR_BUFF_SIZE     (1024U * 50U)
/// hardware slots count
#define SDIO_SLOT_COUNT                     1U
/// maximum supported device(SDIO device drivers) count.
/// Default it is 2 SD and MMC.
#define MAX_SUPPORTED_DEVICE_COUNT          2U
/// option defines if memory card driver should use auto CMD or not.
/// 1 - use auto CMD
/// 0 - don't use auto CMD
#define USE_AUTO_CMD                        0
/// delay in microseconds after power enable
#define POWER_UP_DELAY_US                   2000U
/// number of temporary sub-buffers, used by to split data
/// bigger than 64KB for smaller parts. Split is made by ADMA module.
#define SDIO_CFG_SDIO_SUB_BUFFERS_COUNT     4000U
/// Configuration of how many times each reset operation shall be executed
#define SDIO_CFG_RESET_COUNT                2U
#endif
