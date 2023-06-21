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
 * sdio_errors.c
 * SD Host controller driver error announcements.
 *****************************************************************************/


#include "sdio_types.h"
#include "sdio_errors.h"

#define Err00 "NO Errors\n"
#define Err01 "Hardware problem - host base clock is zero\n"
#define Err02 "Host can't power supply\n"
#define Err03 "Host driver timeout error\n"
#define Err04 "Error during configuration - current limit error\n"
#define Err05 "Error during transfer data - end bit error\n"
#define Err06 "Error during transfer data - crc bit error\n"
#define Err07 "Error during transfer data - timeout error\n"
#define Err08 "Error during execute command - command index error\n"
#define Err09 "Error during execute command - command line conflict\n"
#define Err0A "Error during execute command - timeout error\n"
#define Err0B "Error during execute command - end bit error\n"
#define Err0C "Error during execute command - CRC error\n"
#define Err0D "Card is in inactive state, it is unusable\n"
#define Err0E "Wrong argument for SDIO card function\n"
#define Err0F "Wrong SDIO function number error\n"
#define Err10 "The CRC check for the previous command failed\n"
#define Err11 "Illegal command error\n"
#define Err12 "An error in the sequence of erase commands occurred\n"
#define Err13 "A general or an unknown error occurred during the operation\n"
#define Err14 "A misaligned address that did not match the block length was used in the command\n"
#define Err15 "Can't switch device to high speed mode because it is not supported either by controller or by card\n"
#define Err16 "Command not issued by auto CMD 12 error\n"
#define Err17 "Command index error occurs in response to a command\n"
#define Err18 "End bit of command response is 0\n"
#define Err19 "Error CRC in the Auto CMD 12 command response\n"
#define Err1A "No response is returned within 64 SDCLK cycles from the end bit of command\n"
#define Err1B "Host controller cannot issue Auto CMD12 to stop memory multiple block data transfer due to some error\n"
#define Err1C "Auto CMD12 error - CMD line conflict\n"
#define Err1D "Touple not found in CIS register error\n"
#define Err1E "Found new card device but software driver to service it was not implemented\n"
#define Err1F "Malloc function returns error check your heap settings\n"
#define Err20 "Error, DAT line is busy, can't execute command\n"
#define Err21 "Error, CMD line is busy, can't execute command\n"
#define Err22 "Internal card controller error\n"
#define Err23 "Card internal ECC was applied but failure\n"
#define Err24 "Sequence or password error has been detected in lock/unlock card command\n"
#define Err25 "Host attempts to write to a protected block or to the temporary or permanent write protected card\n"
#define Err26 "An invalid selection of write-blocks for erase occurred\n"
#define Err27 "The transferred block length is not allowed for this card,  \
						or the number of transferred bytes does not match the block length\n"
#define Err28 "The command's s argument was out of the allowed range for this card\n"
#define Err29 "Unrecoverable error ocured\n"
#define Err2A "An error occurred in CMD_wo_DAT, but not in the SD memory transfer\n"
#define Err2B "An error occurred in CMD_wo_DAT, and also occurred in the SD memory transfer\n"
#define Err2C "An error did not occur in CMD_wo_DAT, but an error occurred in the SD memory transfer\n"
#define Err2D "CMD_wo_DAT was not issued, and an error occurred in the SD memory transfer\n"
#define Err2E "Card is card locked by the host\n"
#define Err2F "The card could not sustain data transfer in stream read mode\n"
#define Err30 "The card could not sustain data transfer in stream write mode\n"
#define Err31 "Problem with erase part of memory because it is protected\n"
#define Err32 "Card didn't switch to the expected mode as requested by the SWITCH command\n"
#define Err33 "Error authentication process\n"
#define Err34 "SLOT is busy can't execute a command\n"
#define Err35 "Error command is not supported by the card device\n"
#define Err36 "Device is null pointer\n"
#define Err37 "ADMA error\n"
#define Err38 "Unexpected DMA interrupt (probably wrong descriptor settings)\n"
#define Err39 "Invalid function parameter\n"
#define Err3A "Prohibited operation. Card is lock.\n"
#define Err3B "Error function is not supported (CMD6 command)\n"
#define Err3C "To few memory for ADMA descriptors\n"
#define Err3D "Signal volatge switch procedure failed\n"
#define Err3E "Changing access mode funtion works only when 1.8V signaling level is active\n"
#define Err3F "Operation is unsupported by the driver or core\n"
#define Err40 "Tuning operation failed\n"
#define Err41 "Auto cmd23 is not possible to use. SDMA can't be enabled and card must supports this command\n"
#define Err42 "UHSII initialization failed\n"
#define Err43 "Error detection inside R1 or R5 response\n"
#define Err44 "Error device is initialized\n"
#define Err45 "Header Error is detected in received packet\n"
#define Err46 ""
#define Err47 "Retry Expired - Number of transfer retries reaches Retry Count\n"
#define Err48 "CRC Error is detected in received packet\n"
#define Err49 "Framing Error is detected in received packet\n"
#define Err4A "Transaction ID (TID) Error is detected\n"
#define Err4B "Received packed has the Unrecoverable Error\n"
#define Err4C "Received EBSY Packet has an error\n"
#define Err4D "RES Packet timeout occurs\n"
#define Err4E "Deadlock timeout occurs\n"
#define Err4F "UHSII response error\n"
#define Err50 "CMD error - The CMD is not permitted on the present condition or state\n"
#define Err51 "CMD error - At least one of arguments is improper in the CMD\n"
#define Err52 "CMD error - General error detected\n"
#define Err53 ""
#define Err54 ""
#define Err55 ""
#define Err56 ""
#define Err57 ""
#define Err58 ""
#define Err59 ""
#define Err5A ""
#define Err5B ""
#define Err5C ""
#define Err5D ""
#define Err5E ""
#define Err5F ""
#define Err60 ""
#define Err61 ""
#define Err62 ""
#define Err63 ""
#define Err64 ""
#define Err65 ""
#define Err66 ""
#define Err67 ""
#define Err68 ""
#define Err69 ""
#define Err6A ""
#define Err6B ""
#define Err6C ""
#define Err6D ""
#define Err6E ""
#define Err6F ""
#define Err70 ""
#define Err71 ""
#define Err72 ""
#define Err73 ""
#define Err74 ""
#define Err75 ""
#define Err76 ""
#define Err77 ""
#define Err78 ""
#define Err79 ""
#define Err7A ""
#define Err7B ""
#define Err7C ""
#define Err7D ""
#define Err7E ""
#define Err7F ""
#define Err80 ""
#define Err81 ""
#define Err82 ""
#define Err83 ""
#define Err84 ""
#define Err85 ""
#define Err86 ""
#define Err87 ""
#define Err88 ""
#define Err89 ""
#define Err8A ""
#define Err8B ""
#define Err8C ""
#define Err8D ""
#define Err8E ""
#define Err8F ""
#define Err90 ""
#define Err91 ""
#define Err92 ""
#define Err93 ""
#define Err94 ""
#define Err95 ""
#define Err96 ""
#define Err97 ""
#define Err98 ""
#define Err99 ""
#define Err9A ""
#define Err9B ""
#define Err9C ""
#define Err9D ""
#define Err9E ""
#define Err9F ""
#define ErrA0 ""
#define ErrA1 ""
#define ErrA2 ""
#define ErrA3 ""
#define ErrA4 ""
#define ErrA5 ""
#define ErrA6 ""
#define ErrA7 ""
#define ErrA8 ""
#define ErrA9 ""
#define ErrAA ""
#define ErrAB ""
#define ErrAC ""
#define ErrAD ""
#define ErrAE ""
#define ErrAF ""
#define ErrB0 ""
#define ErrB1 ""
#define ErrB2 ""
#define ErrB3 ""
#define ErrB4 ""
#define ErrB5 ""
#define ErrB6 ""
#define ErrB7 ""
#define ErrB8 ""
#define ErrB9 ""
#define ErrBA ""
#define ErrBB ""
#define ErrBC ""
#define ErrBD ""
#define ErrBE ""
#define ErrBF ""
#define ErrC0 ""
#define ErrC1 ""
#define ErrC2 ""
#define ErrC3 ""
#define ErrC4 ""
#define ErrC5 ""
#define ErrC6 ""
#define ErrC7 ""
#define ErrC8 ""
#define ErrC9 ""
#define ErrCA ""
#define ErrCB ""
#define ErrCC ""
#define ErrCD ""
#define ErrCE ""
#define ErrCF ""
#define ErrD0 ""
#define ErrD1 ""
#define ErrD2 ""
#define ErrD3 ""
#define ErrD4 ""
#define ErrD5 ""
#define ErrD6 ""
#define ErrD7 ""
#define ErrD8 ""
#define ErrD9 ""
#define ErrDA ""
#define ErrDB ""
#define ErrDC ""
#define ErrDD ""
#define ErrDE ""
#define ErrDF ""
#define ErrE0 ""
#define ErrE1 ""
#define ErrE2 ""
#define ErrE3 ""
#define ErrE4 ""
#define ErrE5 ""
#define ErrE6 ""
#define ErrE7 ""
#define ErrE8 ""
#define ErrE9 ""
#define ErrEA ""
#define ErrEB ""
#define ErrEC "Can't execute function\n"
#define ErrED "Driver procedure was called with wrong value of parameter\n"
#define ErrEE "Hardware problem error\n"
#define ErrEF "Undefined error\n"
#define ErrF0 ""
#define ErrF1 ""
#define ErrF2 ""
#define ErrF3 ""
#define ErrF4 ""
#define ErrF5 ""
#define ErrF6 ""
#define ErrF7 ""
#define ErrF8 ""
#define ErrF9 ""
#define ErrFA ""
#define ErrFB ""
#define ErrFC ""
#define ErrFD ""
#define ErrFE "Request was aborted\n"
#define ErrFF "Request is currently executing\n"

const char * SDIO_Errors_GetText(uint8_t ErrorCode)
{
#ifdef DEBUG
    static const char *msgs[] = {
        Err00, Err01, Err02, Err03, Err04, Err05, Err06, Err07, Err08, Err09, Err0A, Err0B, Err0C, Err0D,  Err0E,  Err0F,
        Err10, Err11, Err12, Err13, Err14, Err15, Err16, Err17, Err18, Err19, Err1A, Err1B, Err1C, Err1D,  Err1E,  Err1F,
        Err20, Err21, Err22, Err23, Err24, Err25, Err26, Err27, Err28, Err29, Err2A, Err2B, Err2C, Err2D,  Err2E,  Err2F,
        Err30, Err31, Err32, Err33, Err34, Err35, Err36, Err37, Err38, Err39, Err3A, Err3B, Err3C, Err3D,  Err3E,  Err3F,
        Err40, Err41, Err42, Err43, Err44, Err45, Err46, Err47, Err48, Err49, Err4A, Err4B, Err4C, Err4D,  Err4E,  Err4F,
        Err50, Err51, Err52, Err53, Err54, Err55, Err56, Err57, Err58, Err59, Err5A, Err5B, Err5C, Err5D,  Err5E,  Err5F,
        Err60, Err61, Err62, Err63, Err64, Err65, Err66, Err67, Err68, Err69, Err6A, Err6B, Err6C, Err6D,  Err6E,  Err6F,
        Err70, Err71, Err72, Err73, Err74, Err75, Err76, Err77, Err78, Err79, Err7A, Err7B, Err7C, Err7D,  Err7E,  Err7F,
        Err80, Err81, Err82, Err83, Err84, Err85, Err86, Err87, Err88, Err89, Err8A, Err8B, Err8C, Err8D,  Err8E,  Err8F,
        Err90, Err91, Err92, Err93, Err94, Err95, Err96, Err97, Err98, Err99, Err9A, Err9B, Err9C, Err9D,  Err9E,  Err9F,
        ErrA0, ErrA1, ErrA2, ErrA3, ErrA4, ErrA5, ErrA6, ErrA7, ErrA8, ErrA9, ErrAA, ErrAB, ErrAC, ErrAD,  ErrAE,  ErrAF,
        ErrB0, ErrB1, ErrB2, ErrB3, ErrB4, ErrB5, ErrB6, ErrB7, ErrB8, ErrB9, ErrBA, ErrBB, ErrBC, ErrBD,  ErrBE,  ErrBF,
        ErrC0, ErrC1, ErrC2, ErrC3, ErrC4, ErrC5, ErrC6, ErrC7, ErrC8, ErrC9, ErrCA, ErrCB, ErrCC, ErrCD,  ErrCE,  ErrCF,
        ErrD0, ErrD1, ErrD2, ErrD3, ErrD4, ErrD5, ErrD6, ErrD7, ErrD8, ErrD9, ErrDA, ErrDB, ErrDC, ErrDD,  ErrDE,  ErrDF,
        ErrE0, ErrE1, ErrE2, ErrE3, ErrE4, ErrE5, ErrE6, ErrE7, ErrE8, ErrE9, ErrEA, ErrEB, ErrEC, ErrED,  ErrEE,  ErrEF,
        ErrF0, ErrF1, ErrF2, ErrF3, ErrF4, ErrF5, ErrF6, ErrF7, ErrF8, ErrF9, ErrFA, ErrFB, ErrFC, ErrFD,  ErrFE,  ErrFF
    };
    return msgs[ErrorCode];
#else
    return "";
#endif
}
