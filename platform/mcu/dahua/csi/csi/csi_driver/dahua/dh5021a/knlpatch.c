/*************************************************
  Copyright (C), 2011-2015 ZheJiang Dahua Technology Co., Ltd.
  �ļ���:   knlpatch.c
  ��  ��:   zheng_xingjian(11853)<zheng_xingjian@dahuatech.com>
  ��  ��:   V1.0.0
  ��  �ڣ�  2015-10-24
  ��  ��:   yoc���ں˲����㣬����֧�ֶ�Ӧ��Linux�ں˽ӿڵ���

            1��ʹ��˵��
            ���ļ��У��ⲿ����ʹ��DH_KP_xxx, �ڲ�����ʹ��KP_eCos_xxxx

            2��������
            ��������yoc

  �޶���ʷ:
  1. ��    ��: 2017-11-27
     �޶��汾: v0.1
     ��    ��: zhang_jian
     �޶���ע: ����

  2. ��    ��:
     �޶��汾:
     ��    ��:
     �޶���ע:
*************************************************/
#include <string.h>
#include "knlpatch.h"

/*******************************************************************************
 * ������  : copy_from_user
 * ��  ��  :
 * ��  ��  :  ����        ����
 *         :   to           ������to������
 *         :   from         ������from������
 *         :   n            ������n������
 * ��  ��  : ��
 * ����ֵ  : DH_SUCCESS: �ɹ�
 *           DH_FAILURE: ʧ��
 *******************************************************************************/
unsigned int copy_from_user(void* to,const void __user* from, unsigned int n)
{
    if( NULL == to || NULL == from )
    {
        return 1;
    }

    //to = from;
    memcpy(to,from,n);

    return 0;
}

/*******************************************************************************
 * ������  : copy_to_user
 * ��  ��  :
 * ��  ��  :  ����        ����
 *         :   to           ������to������
 *         :   from         ������from������
 *         :   n            ������n������
 * ��  ��  : ��
 * ����ֵ  : DH_SUCCESS: �ɹ�
 *           DH_FAILURE: ʧ��
 *******************************************************************************/
unsigned int copy_to_user(void __user* to,const void* from, unsigned int n)
{
    if( NULL == to || NULL == from )
    {
        return 1;
    }

    //to = from;
    memcpy(to,from,n);

    return 0;
}

/*******************************************************************************
 * ������  : ioremap
 * ��  ��  :
 * ��  ��  :  ����        ����
 *         :   phys_addr         ������phys_addr������
 *         :   size              ������size������
 * ��  ��  : ��
 * ����ֵ  : DH_SUCCESS: �ɹ�
 *           DH_FAILURE: ʧ��
 *******************************************************************************/
void* ioremap(unsigned int phys_addr, unsigned int size)
{
    return (void *)phys_addr;
}

/*******************************************************************************
 * ������  : iorunmap
 * ��  ��  :
 * ��  ��  :  ����        ����
 *         :   phys_addr         ������phys_addr������
 * ��  ��  : ��
 * ����ֵ  : DH_SUCCESS: �ɹ�
 *           DH_FAILURE: ʧ��
 *******************************************************************************/
void iounmap(void* phys_addr)
{
    return ;
}


