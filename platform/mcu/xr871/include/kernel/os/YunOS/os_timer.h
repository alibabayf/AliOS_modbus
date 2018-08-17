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
#ifndef _KERNEL_OS_YUNOS_OS_TIMER_H_
#define _KERNEL_OS_YUNOS_OS_TIMER_H_

#include "kernel/os/YunOS/os_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	OS_TIMER_ONCE		= 0,
	OS_TIMER_PERIODIC	= 1
} OS_TimerType;

typedef void (*OS_TimerCallback_t)(void *arg);

typedef struct OS_Timer {
	void* handle; /* must be first */
	OS_TimerCallback_t cb;
	OS_TimerType type;
} OS_Timer_t;

static __inline void krhino_timer_cb(void *timer, void *arg)
{
	OS_Timer_t *tmr = timer;
	tmr->cb(arg);
}

kstat_t krhino_timer_dyn_create(ktimer_t **timer, const name_t *name, timer_cb_t cb,
                               tick_t first, tick_t round, void *arg, uint8_t auto_run);

static __inline OS_Status OS_TimerCreate(OS_Timer_t *timer, OS_TimerType type,
                         OS_TimerCallback_t cb, void *ctx, OS_Time_t periodMS)
{
	tick_t first = krhino_ms_to_ticks((uint32_t)periodMS);
	tick_t round = type == OS_TIMER_ONCE ? 0 : first;
#if (RHINO_CONFIG_KOBJ_DYN_ALLOC > 0)
	ktimer_t *timer_obj = NULL;
	if (RHINO_SUCCESS == krhino_timer_dyn_create(&timer_obj, "UNDEF", krhino_timer_cb,
												first, round, ctx, 1)) {
		timer->handle = (void*)timer_obj;
		timer->cb = cb;
		timer->type = type;
		return OS_OK;
	} else {
		timer->handle = NULL;
		timer->cb = NULL;
		return OS_FAIL;
	}
#else
	ktimer_t *timer_obj = = yoc_mm_alloc(sizeof(ktimer_t));
	if (timer_obj == NULL) {
		return OS_FAIL;
	}

	if (RHINO_SUCCESS == krhino_timer_create(timer_obj, "UNDEF", krhino_timer_cb,
												first, round, ctx, 1)) {
		timer->handle = (void*)timer_obj;
		timer->cb = cb;
		return OS_OK;
	} else {
		yoc_mm_free(timer_obj);
		timer->handle = NULL;
		timer->cb = NULL;
		return OS_FAIL;
	}

#endif
}

static __inline OS_Status OS_TimerDelete(OS_Timer_t *timer)
{
#if (RHINO_CONFIG_KOBJ_DYN_ALLOC > 0)
	if (RHINO_SUCCESS == krhino_timer_dyn_del(timer->handle)) {
		timer->handle = NULL;
		timer->cb = NULL;
		return OS_OK;
	} else {
		return OS_FAIL;
	}
#else
	if (RHINO_SUCCESS == krhino_timer_del(timer->handle)) {
		yoc_mm_free(timer->handle);
		timer->handle = NULL;
		timer->cb = cb;
		return OS_OK;
	} else {
		return OS_FAIL;
	}
#endif
}

static __inline OS_Status OS_TimerStart(OS_Timer_t *timer)
{
	if (RHINO_SUCCESS == krhino_timer_start(timer->handle)) {
		return OS_OK;
	} else {
		return OS_FAIL;
	}
}

static __inline OS_Status OS_TimerChangePeriod(OS_Timer_t *timer, OS_Time_t periodMS)
{
	tick_t first = krhino_ms_to_ticks((uint32_t)periodMS);
	tick_t round = timer->type == OS_TIMER_ONCE ? 0 : first;
	if (RHINO_SUCCESS == krhino_timer_change(timer->handle, first, round)) {
		return OS_OK;
	} else {
		return OS_FAIL;
	}
}

static __inline OS_Status OS_TimerStop(OS_Timer_t *timer)
{
	if (RHINO_SUCCESS == krhino_timer_stop(timer->handle)) {
		return OS_OK;
	} else {
		return OS_FAIL;
	}
}

static __inline int OS_TimerIsValid(OS_Timer_t *timer)
{
	return (timer->handle != OS_INVALID_HANDLE);
}

static __inline void OS_TimerSetInvalid(OS_Timer_t *timer)
{
	timer->handle = OS_INVALID_HANDLE;
}

static __inline void *OS_TimerGetContext(void *arg)
{
	return arg;
}

#ifdef __cplusplus
}
#endif

#endif /* _KERNEL_OS_YUNOS_OS_TIMER_H_ */
