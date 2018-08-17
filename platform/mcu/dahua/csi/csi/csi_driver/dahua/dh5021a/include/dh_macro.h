/******************************************************************************

  Copyright (C), 2014-2020, ZheJiang Dahua Technology CO.,LTD.

 ******************************************************************************
  File Name     : dh_macro.h
  Version       : Initial Draft
  Author        : 
  Created       : 2014.3.13
  Last Modified :
  Description   : define the format of error code
  Function List :
  DHstory       :
  1.Date        : 2014/3/13
    Author      :
    Modification: Create
******************************************************************************/

#ifndef _DH_MACRO_H_
#define _DH_MACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ��ʱ���� */
#define DH_TIMEOUT_NONE         (0)    /* ���ȴ����������ء�*/ 
#define DH_TIMEOUT_FOREVER      (~0U)  /* һֱ�ȴ�ֱ������ */

/* ���valueֵ�Ƿ���2��N�η� */
#define DH_checkIs2n(value)  ( ((value) == 0) ? DH_FALSE\
                               : ((value) & ((value) - 1))  \
                                  ? DH_FALSE : DH_TRUE)

/* ���ݴ洢������λ */
#define DH_KB (1024)
#define DH_MB (DH_KB * DH_KB)
#define DH_GB (DH_KB * DH_MB)

/* ��ֵ����*/
#define DH_max(a, b)    ( (a) > (b) ? (a) : (b) )
#define DH_min(a, b)    ( (a) < (b) ? (a) : (b) )
#define DH_max3(a,b,c)   DH_max(DH_max(a,b),c)
#define DH_min3(a,b,c)   DH_min(DH_min(a,b),c)
#define DH_RoundDiv(a,b) (((a)>0 ? (a) + ((b)>>1) : (a) - ((b)>>1))/(b))  /* a ���� b, ��������ȡ��*/
#define DH_abs(a)        ((a) >= 0 ? (a) : (-(a)))
#define DH_swap(type,a,b)  do{type SWAP_tmp= b; b= a; a= SWAP_tmp;}while(0)

/* �ڴ����*/
#define DH_clear(ptr)		           memset((ptr), 0, sizeof(*(ptr)))	
#define DH_clearSize(ptr, size)        memset((ptr), 0, (size))
#define DH_memCmp(dst, src)            memcmp((dst), (src), sizeof(*(src)))
#define DH_memCmpSize(dst, src, size)  memcmp((dst), (src), (size))


/* �������*/
#define DH_ALIGN4(_x)              (((_x)+0x03)&~0x03)
#define DH_ALIGN8(_x)              (((_x)+0x07)&~0x07)
#define DH_ALIGN16(_x)             (((_x)+0x0f)&~0x0f)
#define DH_ALIGN32(_x)             (((_x)+0x1f)&~0x1f)
#define DH_ALIGN64(_x)             (((_x)+0x3f)&~0x3f)
#define DH_align(value, align)   ((( (value) + ( (align) - 1 ) ) \
                                     / (align) ) * (align) )
#define DH_ceil(value, align)    DH_align(value, align) 
#define DH_floor(value, align)   (( (value) / (align) ) * (align) )

/* ������� */
#define DH_setTrue(val)        ((val) = DH_TRUE)
#define DH_setFalse(val)       ((val) = DH_FALSE)      

/* ��ȡ�����Ա���� */
#define DH_arraySize(array)    (sizeof(array)/sizeof((array)[0]))


/* ��ȡ�ṹ���Ա�ĵ�ַƫ���� */
#ifdef __compiler_offsetof
#define DH_offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define DH_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif


/* ͨ���ṹ���Ա��ȡ�ṹ���׵�ַ */
#define DH_containerOf(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - DH_offsetof(type,member) );})
        
#define BITMSK(bit)				(int)(1 << (bit))
#define MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(a)) | ((unsigned short)((unsigned char)(b))) << 8))

#define LOWORD(l)           ((unsigned short)(l))
#define HIWORD(l)           ((unsigned short)(((unsigned int)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((unsigned char)(w))
#define HIBYTE(w)           ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))

#define DH_regReadl(virtAddr) (*((volatile DH_U32*)(virtAddr)))
#define DH_regWritel(virtAddr, value) \
        (*((volatile DH_U32 *)(virtAddr)) = (value))

#define DH_RegRead(RegAddr)             (DH_regReadl(KSEG1ADDR(RegAddr)))
#define DH_RegWrite(RegAddr, RegVal)    (DH_regWritel(KSEG1ADDR(RegAddr), RegVal))

#ifdef __cplusplus
}
#endif

#endif  /*  _DH_MACRO_H_  */

