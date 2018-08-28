//------------------------------------------------------------------------
// Copyright (C) 2016 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : cparm_f8                                            
// Author         : ganghualuo                                                 
// Date of created: 2016-08-22 11:12:11                                                  
// Last modified  : 2016-08-22 11:12:11                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-22 11:12:11 --  Create by ganghualuo                                               
//------------------------------------------------------------------------
#ifndef __CPARM_F8_H__
#define __CPARM_F8_H__
//Auto-gen by reg_gen





//F8_CONF
typedef union
{
    U32 v;
    struct
    {
        U32 F8_START                       :1 ; /*0 :0 , F8�㷨��������λ0��������F8 �㷨������ɺ��Զ����㣻1������F8�㷨 */
        U32 F8_IRQ_EN                      :1 ; /*1 :1 , F8�㷨�ж�ʹ��λ0��F8 �㷨/�������������group�����󣬲������жϣ�1��F8 �㷨/�������������group�����󣬲����ж�*/
        U32 F8_AR_SEL                      :2 ; /*3 :2 , F8�㷨���ͺ�ֻ������ѡ��00��ֻ�����������ӽ���01��AES�ӽ��ܣ�������10��snow3G�ӽ��ܣ�������11��zuc�ӽ��ܣ�������*/
        U32 F8_CONF_reserved_0             :28; /*31:4 , No description.               */
    } b;
} T_CPARM_F8_F8_CONF;

//F8_GROUP_ADDR
typedef union
{
    U32 v;
    struct
    {
        U32 GROUP_ADDR                     :32; /*31:0 , group�׵�ַ                   */
    } b;
} T_CPARM_F8_F8_GROUP_ADDR;

//F8_GROUP_CNT
typedef union
{
    U32 v;
    struct
    {
        U32 GROUP_ADDR                     :32; /*31:0 , group����                     */
    } b;
} T_CPARM_F8_F8_GROUP_CNT;

//F8_STATUS
typedef union
{
    U32 v;
    struct
    {
        U32 F8_STAT                        :1 ; /*0 :0 , 0��F8�㷨/������δ��ɻ�δ��ʼ1��F8�㷨/�����������*/
        U32 F9_STAT                        :1 ; /*1 :1 , 0��F9�㷨/������δ��ɻ�δ��ʼ1��F9�㷨/�����������*/
        U32 F8_STATUS_reserved_0           :30; /*31:2 , No description.               */
    } b;
} T_CPARM_F8_F8_STATUS;

//F9_CONF
typedef union
{
    U32 v;
    struct
    {
        U32 F9_START                       :1 ; /*0 :0 , F9�㷨��������λ0��������F9 �㷨������ɺ��Զ����㣻1������F9�㷨 */
        U32 F9_IRQ_EN                      :1 ; /*1 :1 , F9�㷨�ж�ʹ��λ0��F9 �㷨/�������������group�����󣬲������жϣ�1��F9 �㷨/�������������group�����󣬲����ж�*/
        U32 F9_AR_SEL                      :2 ; /*3 :2 , F9�㷨����ѡ��00��AES�������㷨01��AES�������㷨10��snow3G�������㷨11��zuc�������㷨*/
        U32 F9_CONF_reserved_0             :28; /*31:4 , No description.               */
    } b;
} T_CPARM_F8_F9_CONF;

//F9_GROUP_ADDR
typedef union
{
    U32 v;
    struct
    {
        U32 F9_ADDR                        :32; /*31:0 , F9 group�׵�ַ                */
    } b;
} T_CPARM_F8_F9_GROUP_ADDR;

//F9_RESULT
typedef union
{
    U32 v;
    struct
    {
        U32 F9_MAC                         :32; /*31:0 , F9����Ĵ���                  */
    } b;
} T_CPARM_F8_F9_RESULT;

typedef struct
{
    volatile T_CPARM_F8_F8_CONF              F8_CONF                       ; /*0x0  , RW   , 0x00000000, F8���üĴ���                  */
    volatile T_CPARM_F8_F8_GROUP_ADDR        F8_GROUP_ADDR                 ; /*0x4  , RW   , 0x00000000, F8 group�׵�ַ�Ĵ���          */
    volatile T_CPARM_F8_F8_GROUP_CNT         F8_GROUP_CNT                  ; /*0x8  , RW   , 0x00000000, F8 group�����Ĵ���            */
    volatile T_CPARM_F8_F8_STATUS            F8_STATUS                     ; /*0xc  , RW   , 0x00000000, F8״̬�Ĵ���                  */
    volatile T_CPARM_F8_F9_CONF              F9_CONF                       ; /*0x10 , RW   , 0x00000000, F9���üĴ���                  */
    volatile T_CPARM_F8_F9_GROUP_ADDR        F9_GROUP_ADDR                 ; /*0x14 , RW   , 0x00000000, F9 group�׵�ַ�Ĵ���          */
    volatile T_CPARM_F8_F9_RESULT            F9_RESULT                     ; /*0x18 , RO   , 0x00000000, F9 ����Ĵ���                 */
} T_HWP_CPARM_F8_T;


//F8_CONF
#define F8_CONF_F8_START               (1<<0)              

#define F8_CONF_F8_IRQ_EN              (1<<1)              

#define F8_CONF_F8_AR_SEL(n)           (((n)&0x3)<<2)      


//F8_GROUP_ADDR
#define F8_GROUP_ADDR_GROUP_ADDR(n)    (((n)&0xFFFFFFFF)<<0)


//F8_GROUP_CNT
#define F8_GROUP_CNT_GROUP_ADDR(n)     (((n)&0xFFFFFFFF)<<0)


//F8_STATUS
#define F8_STATUS_F8_STAT              (1<<0)              

#define F8_STATUS_F9_STAT              (1<<1)              


//F9_CONF
#define F9_CONF_F9_START               (1<<0)              

#define F9_CONF_F9_IRQ_EN              (1<<1)              

#define F9_CONF_F9_AR_SEL(n)           (((n)&0x3)<<2)      


//F9_GROUP_ADDR
#define F9_GROUP_ADDR_F9_ADDR(n)       (((n)&0xFFFFFFFF)<<0)


//F9_RESULT
#define F9_RESULT_F9_MAC(n)            (((n)&0xFFFFFFFF)<<0)

#define  F8_LTE_F8      0x01
#define  F8_LTE_F9      0x02



typedef struct                       
{
    u32 handle;                                           /*ģ���־*/     
    u32 open_flg;                                        /*ģ�鿪����־*/             
    void (*txcallback)(void *pbuff, u32 length);          /*�ص�����ָ��*/
    void (*rxcallback)(void *pbuff, u32 length);          /*�ص�����ָ��*/
    u32 reserve_len;  
    u32 rx_max_len;                                       /*ĳ��ҵ����տ��ܵ�������ݸ��������ڶ�̬�����ڴ���*/
}HAL_MODULES_INFO_T, *pHAL_MODULES_INFO_T;


#endif


