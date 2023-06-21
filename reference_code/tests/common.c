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
 * Common file for test
 *****************************************************************************/

#include <stdio.h>
#include <common.h>


unsigned char *writeBuffer;
unsigned char *readBuffer;

/* size has to be divisible by 4 */
void Clearbuf(void *buffer, uint32_t size,uint32_t dataPattern)
{
    uint32_t *buf = buffer;
    uint32_t sizeWords = size / 4;
    uint32_t *bufEnd = buf + sizeWords ;

    while (buf < bufEnd)
        *buf++ = dataPattern;
}

/* size has to be divisible by 4 */
void Fillbuf(void *buffer, uint32_t size, uint32_t start)
{
    uint32_t *buf = buffer;
    uint32_t sizeWords = size / 4;
    uint32_t *bufEnd = buf + sizeWords;

    start *= 128;
    while (buf < bufEnd){
        *buf++ = (start / 128) | ((start / 128) << 8) | ((start / 128) << 16) | ((start / 128) << 24);
    start++;
    }
}

uint32_t Comparebuf(void *b1, void *b2, uint32_t size)
{
    int i;
    uint8_t status = 0;
    unsigned char *buffer1 = b1, *buffer2 = b2;

    for (i = 0; i < size; i++) {
        if (buffer1[i] != buffer2[i]) {
            printf("[%d] = 0x%x, [%d] = 0x%x\n", i, buffer1[i],
                   i, buffer2[i]);
            status = 0x90;
        }
    }
    return status;
}

uint8_t PrintBuffers(int size)
{
    int i = 0;

    SubPrint("Written data:\n");
    for (i = 0; i < size; i++) {
        if ((i % 48) == 0) {
            SubPrint("\n");
        }
        SubPrint("%*X",2, writeBuffer[i]);
    }

    SubPrint("\nRead data:\n");
    for (i = 0; i < size; i++) {
        if ((i % 48) == 0) {
            SubPrint("\n");
        }
        SubPrint("%*X",2, readBuffer[i]);
    }

    SubPrint("\n");
    return 0;
}

/* size has to be divisible by 4 */
void PrintBuffers2(void *buffer1, void *buffer2, uint32_t size)
{
    uint32_t *buf1 = buffer1, *buf2 = buffer2;
    uint32_t pos = 0, i = 0;
    uint32_t sizeWords = size / 4;
    printf("---- comparing buffers ----\n");

    while(sizeWords){
        switch(sizeWords){
        case 3:
            printf("%04x: %08x,%08x,%08x -- %08x,%08x,%08x\n",
                   pos, buf1[i], buf1[i + 1], buf1[i + 2],
                   buf2[i], buf2[i + 1], buf2[i + 2]);
            sizeWords -= 3;
            break;
        case 2:
            printf("%04x: %08x,%08x -- %08x,%08x\n",
                   pos, buf1[i], buf1[i + 1],
                   buf2[i], buf2[i + 1]);
            sizeWords -= 2;
            break;
        case 1:
            printf("%04x: %08x -- %08x\n",
                   pos, buf1[i], buf2[i]);
            sizeWords -= 1;
            break;
        default:
            printf("%04x: %08x,%08x,%08x,%08x -- %08x,%08x,%08x,%08x\n",
                   pos, buf1[i], buf1[i + 1], buf1[i + 2], buf1[i + 3],
                   buf2[i], buf2[i + 1], buf2[i + 2], buf2[i + 3]);
            sizeWords -= 4;
            break;
        }
        i += 4;
        pos += 16;
    }
    printf("----        end        ----\n");
}


