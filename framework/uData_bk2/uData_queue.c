

#include <stdio.h>
#include <string.h>
#include "uData_queue.h"


#define UDATA_QUEUE_MAXSLOTS 8
#define UDATA_TASK_STACK_SIZE 4096 // 4kByte
#define UDATA_TASK_PRIO \
    (AOS_DEFAULT_APP_PRI - 2) // higher prio than normal app's
#define UDATA_QUEUE_MAX_MSG_SIZE (sizeof(sensor_msg_pkg_t))
#define UDATA_QUEUE_MAX_MSG_COUNT (32)
#define UDATA_QUEUE_SIZE (UDATA_QUEUE_MAX_MSG_SIZE * UDATA_QUEUE_MAX_MSG_COUNT)

aos_queue_t             g_uData_queue;
static uData_queue_cb_t g_uData_queue_cb[UDATA_QUEUE_MAXSLOTS];
char                    g_uData_msg[UDATA_QUEUE_SIZE] = { 0 };
aos_task_t              g_uData_task;

static void uData_msg_dispatcher(void *arg)
{

    int  ret = 0;
    char rec_cached[256];
    // all the cmd of sensorhub will be sent to be handled here;
    // the dispatcher will asign the new sub task to the fitted model
    while (DO_FOREVER) {
        uint32_t          size = 0;
        sensor_msg_pkg_t *msg  = NULL;
        ret = aos_queue_recv(&g_uData_queue, AOS_WAIT_FOREVER,
                             (void *)rec_cached, (int *)(&size));

        if (size == 0) {
            continue;
        }

        msg = (sensor_msg_pkg_t *)rec_cached;
        for (int n = 0; n < UDATA_QUEUE_MAXSLOTS; n++) {
            if (g_uData_queue_cb[n].status == UDATA_QUEUE_OPEN) {
                g_uData_queue_cb[n].msg_cb(msg);
            }
        }
    }
}

int uData_msg_dispatcher_task_create(void)
{
    int ret = 0;

    ret =
      aos_task_new_ext(&g_uData_task, "uData_queue_task", uData_msg_dispatcher,
                       NULL, UDATA_TASK_STACK_SIZE, UDATA_TASK_PRIO);
    if (unlikely(ret)) {
        return -1;
    }

    return 0;
}

int uData_register_msg_handler(void *func)
{
    int index;
    if (func == NULL) {
        return -1;
    }

    for (index = 0; index < UDATA_QUEUE_MAXSLOTS; index++) {
        if (g_uData_queue_cb[index].status == UDATA_QUEUE_CLOSE) {
            g_uData_queue_cb[index].status = UDATA_QUEUE_OPEN;
            g_uData_queue_cb[index].msg_cb = func;
            return index;
        }
    }
    return -1;
}


int uData_unregister_msg_handler(int index)
{
    if (index >= UDATA_QUEUE_MAXSLOTS || index <= 0)
        return -1;
    g_uData_queue_cb[index].msg_cb = NULL;
    g_uData_queue_cb[index].status = UDATA_QUEUE_CLOSE;
    return 0;
}

int uData_post_msg(sensor_msg_pkg_t msg)
{
    int ret;
    ret = aos_queue_send(&g_uData_queue, (void *)&msg, sizeof(msg));
    if (unlikely(ret)) {
        return -1;
    }
    return 0;
}


int uData_start()
{
    int ret;
    memset(g_uData_queue_cb, 0,
           UDATA_QUEUE_MAXSLOTS * sizeof(uData_queue_cb_t));
    // create the buf queue for the service dispatcher
    ret = aos_queue_new(&g_uData_queue, (void *)&g_uData_msg, UDATA_QUEUE_SIZE,
                        UDATA_QUEUE_MAX_MSG_SIZE);
    if (unlikely(ret)) {
        return -1;
    }

    // create the service dispatcher task
    ret = uData_msg_dispatcher_task_create();
    if (unlikely(ret)) {
        return -1;
    }
    return 0;
}
