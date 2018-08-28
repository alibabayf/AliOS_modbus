#ifndef UDATA_QUEUE
#define UDATA_QUEUE
#include "uData_com_desc.h"

int uData_post_msg(sensor_msg_pkg_t msg);
int uData_unregister_msg_handler(int index);
int uData_register_msg_handler(void *func);
int uData_start();

#endif
