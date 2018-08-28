/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * @file     drv_timer.h
 * @brief    header file for timer driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#ifndef _CSI_TIMER_H_
#define _CSI_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <drv_common.h>
#include "dh_type.h"

typedef enum tagTimerMode
{
    TIMER_MODE_ONETIME = 0,
    TIMER_MODE_RELOAD  = 1
} TIMER_MODE_E;



/*******************************************************************************
* ������  : TIMER_DRV_ConfigMode
* ��  ��  :
* ��  ��  : - u32TimerId:��ʱ��ID
* ��  ��  : ��
* ����ֵ  : DH_SUCCESS  : �ɹ�
*           DH_FAILURE: ʧ��
*******************************************************************************/
DH_S32 TIMER_DRV_ConfigMode(DH_U32 u32TimerId, DH_U32 u32Mode);

/*******************************************************************************
* ������  : TIMER_DRV_Start
* ��  ��  : ������ʱ��
* ��  ��  : - u32TimerId:��ʱ��ID
*         : - timeout:��ʱʱ�� ΢��(us)Ϊ��λ
* ��  ��  : ��
* ����ֵ  : DH_SUCCESS  : �ɹ�
*           DH_FAILURE: ʧ��
*******************************************************************************/
DH_S32 TIMER_DRV_Start(DH_U32 u32TimerId, DH_U32 timeout);

/*******************************************************************************
* ������  : TIMER_DRV_Stop
* ��  ��  :
* ��  ��  : - u32TimerId:��ʱ��ID
* ��  ��  : ��
* ����ֵ  : DH_SUCCESS  : �ɹ�
*           DH_FAILURE: ʧ��
*******************************************************************************/
DH_S32 TIMER_DRV_Stop(DH_U32 u32TimerId);


/*******************************************************************************
*  ������   : TIMER_DRV_Reload
*  ��  ��   : ��RELOADģʽ�£�ʵʱ�޸Ķ�ʱ����
*  ��  ��   : hTimer: ��ʱ�����
*           : u32Timeout: ��ʱ���� (΢��)
*  ��  ��   : �ޡ�
*  ����ֵ   : DH_SUCCESS: �ɹ���
*             DH_FAILURE: ʧ�ܡ�
*******************************************************************************/
DH_S32 TIMER_DRV_Reload(DH_U32 u32TimerId, DH_U32 u32Timeout);



/*******************************************************************************
* ������  : TIMER_DRV_Reset
* ��  ��  :
* ��  ��  : - u32TimerId:��ʱ��ID
* ��  ��  : ��
* ����ֵ  : DH_SUCCESS  : �ɹ�
*           DH_FAILURE: ʧ��
*******************************************************************************/
DH_S32 TIMER_DRV_Reset(DH_U32 u32TimerId);


#ifdef __cplusplus
}
#endif

#endif /* _CSI_TIMER_H_ */

