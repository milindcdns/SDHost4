####################################################################
#
# (C) 2023 Cadence Design Systems, Inc. 
#
####################################################################
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
####################################################################
#
# Test build makefile for HPNFC driver reference tests
####################################################################

# assume we're in the ref_code/tests directory
CORE_DIR ?= ../../core_driver
CCP_DIR ?= ../../ccp_driver
DRV_LIB = $(CORE_DIR)/lib/libsdhc.a
CCP_DRV_LIB = $(CCP_DIR)/lib/libccp.a

CFLAGS := -I./
CFLAGS += -I$(CORE_DIR)/include
CFLAGS += -I$(CORE_DIR)/common
CFLAGS += -I$(CORE_DIR)/src
CFLAGS += -I$(CCP_DIR)/include
CFLAGS += -I$(CCP_DIR)/src
CFLAGS += -D __BARE_METAL__

# TODO: set CC appropriately to target platform
#CC := arm-none-eabi-gcc
CC := gcc

common_files := common.o  cps_bm.o  sdio_dfi.o $(DRV_LIB) $(CCP_DRV_LIB)

tests: boot_test refcode

.PHONY: all
all:  tests

drvlib: $(DRV_LIB)

$(DRV_LIB):
	@echo "*** building core driver library ***"
	make -C $(CORE_DIR)
	@echo "*** building core driver library DONE***"

$(CCP_DRV_LIB):
	@echo "*** building CCP driver library ***"
	make -C $(CCP_DIR)
	@echo "*** building CCP driver library DONE***"



refcode: refcode.o $(common_files) 
boot_test: boot_test.o  $(common_files) 

clean:
	rm -f refcode boot_test *.o 


