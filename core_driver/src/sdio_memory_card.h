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
 * sdio_memory_card.h
 * SD Host controller driver - Memory card driver
 *****************************************************************************/


#ifndef SD_MMC_CARD_H
#define SD_MMC_CARD_H

#include "sdio_types.h"
#include "csdd_if.h"

struct MEMORY_CARD_INFO_s {
    /// Block size
    uint16_t BlockSize;
    /// Card command classes
    uint16_t commandClasses;
    /// Device size
    uint32_t DeviceSizeMB;
    /// Defines whether partial block sizes can be used in block read commands.
    uint8_t PartialReadAllowed;
    /// Defines whether partial block sizes can be used in block write commands.
    uint8_t PartialWriteAllowed;
    /// Defines if the data block to be written by one command can be
    /// spread over more than one physical block of the memory device.
    uint8_t WriteBlkMisalign;
    /// Defines if the data block to be read by one command can be
    /// spread over more than one physical block of the memory device.
    uint8_t ReadBlkMisalign;
    /// The size of an erasable sector.
    /// The content of this register is a 7-bit binary coded value, defining the
    /// number of write blocks (see WRITE_BL_LEN
    uint16_t SectorSize;
    /// The EraseBlkEn defines the granularity of the unit size of the data to be erased.
    uint8_t EraseBlkEn;
    /// The maximum read data block length.
    uint8_t ReadBlLen;
    /// The maximum write data block length.
    uint8_t WriteBlLen;
};

/***************************************************************/
/*!
 * @fn       void MemoryCard_LoadDriver(void)
 * @brief   Function loads memory card driver to host drivers list
 */
/***************************************************************/
void MemoryCard_LoadDriver(void);

/***************************************************************/
/*!
 * @fn      uint8_t MemoryCard_DataXfer( CSDD_SDIO_Device* pDevice,
 *                                        uint32_t Address,
 *                                        uint8_t *Buffer,
 *                                        uint32_t BufferSize,
 *                                        uint8_t TransferDirection )
 * @brief   Function transfers data to/from memory card.
 *              Function operates on 512 data blocks
 * @param   pDevice Device card to which data shall be send
 * @param   Address Addres in 512 bytes blocks on memory card where data
 *              shall be transfer to/from.
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes.
 *              Buffer size should be divisible by 512
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions
 *              to find something out
 * @return	Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/***************************************************************/
uint8_t MemoryCard_DataXfer( CSDD_SDIO_Device*           pDevice,
                                 uint32_t               Address,
                                 uint8_t *              Buffer,
                                 uint32_t               BufferSize,
                                 CSDD_TransferDirection TransferDirection );

/***************************************************************/
/*!
 * @fn      uint8_t MemoryCard_DataXfer2( CSDD_SDIO_Device* pDevice,
 *                                        uint32_t Address,
 *                                        void* Buffer,
 *                                        uint32_t BufferSize,
 *                                        CSDD_TransferDirection TransferDirection,
 *                                        uint32_t SubBufferCount )
 * @brief   Function transfers data to/from memory card.
 *              Function operates on 512 data blocks
 * @param   pDevice Device card to which data shall be send
 * @param   Address Addres in 512 bytes blocks on memory card where data
 *              shall be transfer to/from.
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes.
 *              Buffer size should be divisible by 512
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions
 *              to find something out
 * @param   SubBufferCount if parameter is bigger than 0
 *		the Buffer parameter is treated as array of CSDD_SubBuffer
 *		pointers.
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/***************************************************************/
uint8_t MemoryCard_DataXfer2( CSDD_SDIO_Device*           pDevice,
                                  uint32_t               Address,
                                  void*                  Buffer,
                                  uint32_t               BufferSize,
                                  CSDD_TransferDirection TransferDirection,
                                  uint32_t               SubBufferCount );

/***************************************************************/
/*!
 * @fn      uint8_t MemoryCard_Configure ( CSDD_SDIO_Device* pDevice, CSDD_MmcConfigCmd Cmd,
 *                                      const uint8_t* Data, uint8_t SizeOfData )
 * @brief   Function configures card
 * @param   pDevice Device card which shall be configure
 * @param   Cmd This command defines what operation will be execute.
 *              All commands are defined here @ref CardConfigureCommands
 * @param   Data If some data are need to execute Cmd command then they should
 *              be placed in Data buffer
 * @param   SizeOfData Size of Data in bytes
 * @return	Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/***************************************************************/
uint8_t MemoryCard_Configure ( CSDD_SDIO_Device* pDevice, CSDD_MmcConfigCmd Cmd,
                               const uint8_t* Data, uint8_t SizeOfData );

/***************************************************************/
/*!
 * @fn      static uint8_t MemoryCard_GetCSD( CSDD_SDIO_Device* pDevice )
 * @brief   Function extracts some information from CSD register
                and writes them to pDevice variable.
 * @param   pDevice Device on which operation shall be executed
 * @return	Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/***************************************************************/
//static uint8_t MemoryCard_GetCSD( CSDD_SDIO_Device* pDevice )

/***************************************************************/
/*!
 * @fn      uint8_t MemoryCard_DataErase(CSDD_SDIO_Device* pDevice,
 *                        uint32_t StartBlockAddress,
 *                        uint32_t BlockCount)
 * @brief   Function erases specyfied blocks
 * @param   pDevice Device card which shall be configure
 * @param   StartBlockAddress
 * @param   BlockCount
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/***************************************************************/
uint8_t MemoryCard_DataErase(CSDD_SDIO_Device* pDevice,
                             uint32_t     StartBlockAddress,
                             uint32_t     BlockCount);

/***************************************************************/
/*!
 * @fn      uint8_t MemoryCard_PartialDataTransfer(CSDD_SDIO_Device* pDevice,
                                    uint32_t Address,
                                    void *Buffer,
                                    uint32_t BufferSize,
                                    CSDD_TransferDirection TransferDirection)
 * @brief   Function transfers data to/from memory card.
 *              Function operates on bytes
 * @param   pDevice Device card to which data shall be send
 * @param   Address Addres in bytes on memory card where data
 *              shall be transfer to/from.
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes.
 *              Buffer size should be divisible by 512
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions
 *              to find something out
 * @return	Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/***************************************************************/
uint8_t MemoryCard_PartialDataTransfer(CSDD_SDIO_Device*      pDevice,
                                       uint32_t               Address,
                                       void*                  Buffer,
                                       uint32_t               BufferSize,
                                       CSDD_TransferDirection TransferDirection);

/*****************************************************************************/
/*!
 * @fn      uint8_t MemoryCard_InfXferStart(CSDD_SDIO_Device* pDevice,
 *                                uint32_t Address, void* Buffer,
 *                        uint32_t BufferSize,
                          CSDD_TransferDirection TransferDirection)
 * @brief   Function transfers data in infinite mode to/from memory card.
 *              Function operates on 512 data blocks.
 * @param   pDevice Device card to which data shall be send
 * @param   Address address in card memory to/from which data will
 *              be transferred; address in blocks (512 bytes);
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes.
 *              Buffer size should be divisible by 512
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions
 *              to find something out
 * @return  Function returns 0 if everything is OK
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t MemoryCard_InfXferStart(CSDD_SDIO_Device* pDevice, uint32_t Address,
                                         void* Buffer, uint32_t BufferSize,
                                         CSDD_TransferDirection TransferDirection);

/*****************************************************************************/
/*!
 * @fn      uint8_t MemoryCard_InfXferContinue(CSDD_SDIO_Device* pDevice,
 *                                         void* Buffer, uint32_t BufferSize,
 *                                         CSDD_TransferDirection TransferDirection)
 * @brief   Function continues data transfer in infinite mode and transfers
 *          to/from memory card. Function operates on 512 data blocks.
 * @param   pDevice Device card to which data shall be send
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes.
 *              Buffer size should be divisible by 512
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions
 *              to find something out
 * @return  Function returns 0 if everything is OK
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t MemoryCard_InfXferContinue(CSDD_SDIO_Device* pDevice,
                                            void* Buffer, uint32_t BufferSize,
                                            CSDD_TransferDirection TransferDirection);

/*****************************************************************************/
/*!
 * @fn      uint8_t MemoryCard_InfDataXferFinish(CSDD_SDIO_Device* pDevice,
 *						  CSDD_TransferDirection TransferDirection)
 * @brief   Function finishes data transfer in infinite mode.
 * @param   pDevice Device card to which finish command shall be send
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions
 *              to find something out
 * @return  Function returns 0 if everything is OK
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t MemoryCard_InfDataXferFinish(CSDD_SDIO_Device*           pDevice,
                                             CSDD_TransferDirection TransferDirection);

/*****************************************************************************/
/*!
 * @fn      uint8_t MemoryCard_DataXferNonBlock(CSDD_SDIO_Device* pDevice,
 *                                              uint32_t Address, void* Buffer,
 *                                              uint32_t BufferSize,
 *                                              CSDD_TransferDirection TransferDirection,
 *                                              void **Request)
 * @brief   Function transfers data to/from memory card.
 *              Function operates on 512 data blocks. Function don't wait on
 *              operation finish. Therefore it returns pointer to current request
 *              User using MemoryCard_FinishXferNonBlock function and
 *              request pointer can wait until operation finish
 *              and  get the status of operation. Function needs
 *              AUTO_CMD option enabled.
 * @param   pDevice Device card to which data shall be send
 * @param   Address Addres in 512 bytes blocks on memory card where data
 *              shall be transfer to/from.
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes.
 *              Buffer size should be divisible by 512
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions
 *              to find something out
 * @param   Request defining transfer operation
 * @return  Function returns 0 if everything is ok
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t MemoryCard_DataXferNonBlock(CSDD_SDIO_Device* pDevice, uint32_t Address,
                                        void* Buffer, uint32_t BufferSize,
                                        CSDD_TransferDirection TransferDirection,
                                        void **Request);

/*****************************************************************************/
/*!
 * @fn      uint8_t MemoryCard_FinishXferNonBlock(CSDD_Request* pRequest)
 * @brief   Function waits until request finish
 *          and returns status of request execution
 * @param   pRequest request to wait on
 * @return  Function returns 0 if everything is OK
 *              otherwise returns error number
 */
/*****************************************************************************/
uint8_t MemoryCard_FinishXferNonBlock(CSDD_Request* pRequest);

uint8_t MemoryCard_GetSecCount(const CSDD_SDIO_Device* pDevice, uint32_t *sectorCount);

#endif
