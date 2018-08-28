/*
 * Copyright (C) 2017 XRADIO TECHNOLOGY CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of XRADIO TECHNOLOGY CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SYS_DUCC_DUCC_ADDR_H_
#define _SYS_DUCC_DUCC_ADDR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* There are 4 types of memory address value for app core and net core.
 *   - APP_MEM_APP_ADDR: memory allocated from app, used by app
 *   - APP_MEM_NET_ADDR: memory allocated from app, used by net
 *   - NET_MEM_APP_ADDR: memory allocated from net, used by app
 *   - NET_MEM_NET_ADDR: memory allocated from net, used by net
 */

/* convert from APP_MEM_APP_ADDR to APP_MEM_NET_ADDR */
#define DUCC_APPMEM_APP2NET(addr)	((uint32_t)(addr) | 0x20000000)

/* convert from APP_MEM_NET_ADDR to APP_MEM_APP_ADDR */
#define DUCC_APPMEM_NET2APP(addr)	((uint32_t)(addr) & ~0xf0000000)

/* convert from NET_MEM_NET_ADDR to NET_MEM_APP_ADDR */
#define DUCC_NETMEM_NET2APP(addr)	((uint32_t)(addr) | 0x60000000)

/* convert from NET_MEM_APP_ADDR to NET_MEM_NET_ADDR */
#define DUCC_NETMEM_APP2NET(addr)	((uint32_t)(addr) & ~0xf0000000)

#ifdef __cplusplus
}
#endif

#endif	/* _SYS_DUCC_DUCC_ADDR_H_ */
