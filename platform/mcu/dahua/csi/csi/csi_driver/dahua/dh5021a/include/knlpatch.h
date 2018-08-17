/*************************************************
  Copyright (C), 2011-2015 ZheJiang Dahua Technology Co., Ltd.
  �ļ���:   init.h
  ��  ��:   zheng_xingjian(11853)<zheng_xingjian@dahuatech.com>
  ��  ��:   V1.0.0
  ��  �ڣ�  2015-10-24
  ��  ��:   ϵͳ��ʼ�����ͷ�ļ�������eCos3.0��Linux�ں˵���ֲ������

            1��ʹ��˵��
            ��

            2��������
            ��������eCos3.0

  �޶���ʷ:
  1. ��    ��:
     �޶��汾:
     ��    ��:
     �޶���ע:

  2. ��    ��:
     �޶��汾:
     ��    ��:
     �޶���ע:
*************************************************/
#ifndef _KNLPATCH_H_
#define _KNLPATCH_H_


#define I2C_M_RD    (0x0001)
#define __init
#define __exit
#define __user
#define __iomem


/*�ڴ濽��*/
unsigned int copy_from_user(void* to,const void __user* from, unsigned int n);
unsigned int copy_to_user(void __user* to,const void* from, unsigned int n);

void* ioremap(unsigned int phys_addr, unsigned int size);
void iounmap(void* phys_addr);

#endif
