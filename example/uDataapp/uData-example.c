/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "aos/log.h"
#include "aos/yloop.h"
#include "aos/network.h"
#include <aos/kernel.h>
#include <aos/cli.h>
#include <aos/cloud.h>
#include "mqtt_instance.h"
     
#ifdef AOS_ATCMD
#include <atparser.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <aos/aos.h>
#include <aos/yloop.h>
#include "netmgr.h"
#include "aos/uData.h"

#ifdef DATA_TO_CLOUD

#include "iot_import.h"
#include "iot_export.h"
#include "iot_export_mqtt.h"
#ifdef AOS_ATCMD
#include <atparser.h>
#endif
#ifdef CSP_LINUXHOST
#include <signal.h>
#endif

#endif/*end DATA_TO_CLOUD*/


#define UDATA_PRINT    printf

#define UDATA_SHOW_UINT_1(TYPE,TIME,DATA1) \
do{\
    UDATA_PRINT("uData_application::::::::::::::type = (%d)\n", (TYPE)); \
    UDATA_PRINT("uData_application::::::::::::::data = (%d)\n", (DATA1)); \
    UDATA_PRINT("uData_application:::::::::timestamp = (%d)\n", (uint32_t)(TIME)); \
}while(0);

#define UDATA_SHOW_UINT_3(TYPE,TIME,DATA1,DATA2,DATA3) \
do{\
    UDATA_PRINT("uData_application::::::::::::::type = (%d)\n", (TYPE)); \
    UDATA_PRINT("uData_application::::::::::::::data = (%d) (%d) (%d)\n", (DATA1),(DATA2),(DATA3)); \
    UDATA_PRINT("uData_application:::::::::timestamp = (%d)\n", (uint32_t)(TIME)); \
}while(0);

#define UDATA_SHOW_FLOAT_3(TYPE,TIME,DATA1,DATA2,DATA3) \
do{\
    UDATA_PRINT("uData_application::::::::::::::type = (%d)\n", (TYPE)); \
    UDATA_PRINT("uData_application::::::::::::::data = (%f) (%f) (%f)\n", (DATA1),(DATA2),(DATA3)); \
    UDATA_PRINT("uData_application:::::::::timestamp = (%d)\n", (uint32_t)(TIME)); \
}while(0);



typedef struct {
    char productKey[16];
    char deviceName[32];
    char deviceSecret[48];

    int max_msg_size;
    int max_msgq_size;
    int connected;
    int (*event_handler)(int event_type, void *ctx);
    int (*delete_subdev)(char *productKey, char *deviceName, void *ctx);
    void *ctx;
} MqttContext;

// These are pre-defined topics
#define TOPIC_UPDATE            "/"PRODUCT_KEY"/"DEVICE_NAME"/update"
#define TOPIC_ERROR             "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"
#define TOPIC_GET               "/"PRODUCT_KEY"/"DEVICE_NAME"/get"
#define TOPIC_DATA            "/"PRODUCT_KEY"/"DEVICE_NAME"/soildata"
#define MSG_LEN_MAX             (2048)

int cnt = 0;
static int is_subscribed = 0;

#ifdef MQTT_PRESS_TEST
static int sub_counter = 0;
static int pub_counter = 0;
#endif
char msg_pub[128];
uint32_t g_data[4]={0};
int g_pub =0;
/*
static void ota_init(void *P)
{
    aos_post_event(EV_SYS, CODE_SYS_ON_START_FOTA, 0u);
}
*/
static void mqtt_sub_callback(char *topic, int topic_len, void *payload, int payload_len, void *ctx)
{
    LOG("----");
    LOG("Topic: '%.*s' (Length: %d)",
        topic_len,
        topic,
        topic_len);
    LOG("Payload: '%.*s' (Length: %d)",
        payload_len,
        (char *)payload,
        payload_len);
    LOG("----");

#ifdef MQTT_PRESS_TEST
    sub_counter++;
    int rc = mqtt_publish(TOPIC_UPDATE, IOTX_MQTT_QOS1, payload, payload_len);
    if (rc < 0) {
        LOG("IOT_MQTT_Publish fail, ret=%d", rc);
    } else {
        pub_counter++;
    }
    LOG("RECV=%d, SEND=%d", sub_counter, pub_counter);
#endif MQTT_PRESS_TEST
}

void uData_report_demo(sensor_msg_pkg_t *msg)
{
    udata_pkg_t buf;
    if (msg == NULL) {
        return;
    }


    int rc = -1;
    int msg_len;
#if 0
    if (is_subscribed == 0) {
        /* Subscribe the specific topic */
        rc = mqtt_subscribe(TOPIC_GET, mqtt_sub_callback, NULL);
        if (rc < 0) {
            // IOT_MQTT_Destroy(&pclient);
            LOG("IOT_MQTT_Subscribe() failed, rc = %d", rc);
        }
        is_subscribed = 1;
        //aos_schedule_call(ota_init, NULL);
    }
#endif

    if (msg->cmd==UDATA_MSG_REPORT_PUBLISH) {
        int ret = 0;
        ret = uData_report_publish(msg, &buf);
        if (ret != 0) {
            return;
        }
        int msg_len;
        integer_data_t *m_data =(integer_data_t*)buf.payload;
        switch (buf.type) {
            
            case UDATA_SERVICE_WINDSPD: {
                g_data[0] = m_data->data;
                break;
            }

            case UDATA_SERVICE_WINDDIR: {
               
                g_data[1] = m_data->data;
                break;
            }

            case UDATA_SERVICE_RAIN: {
                
                g_data[2] = m_data->data;
                break;
            }

            case UDATA_SERVICE_TODAYRAIN: {
                
                g_data[3] = m_data->data;
                if(g_pub ==1){
                    g_data[1] = g_data[1] /10;
                    if(g_data[1]>180) g_data[1]= g_data[1] -180;
                    else g_data[1] = g_data[1] + 180;
                    LOG("yftest g_pub!!!!!!!!!!!!!!!!!!!!!!!!!!!!!,%d,%d,%d,%d\n",g_data[0],g_data[1],g_data[2],g_data[3]);
                    msg_len = snprintf(msg_pub, sizeof(msg_pub),  "{\"WINDSPD\":%d,\"WINDDIR\":%d,\"RAIN\":%d,\"TODAYRAIN\":%d}", g_data[0],g_data[1],g_data[2],g_data[3]);
                    if (msg_len < 0) {
                        LOG("Error occur! Exit program");
                    }
                    int rc = mqtt_publish(TOPIC_DATA, IOTX_MQTT_QOS1, msg_pub, msg_len);
                    if (rc < 0) {
                        LOG("error occur when publish");
                    }
                    g_pub =0;
                }
                break;
            }

            default:
                break;

        }
        
    }
}

#if 0
void uData_report_demo(sensor_msg_pkg_t *msg)
{
    udata_pkg_t buf;
    if (msg == NULL) {
        return;
    }


    int rc = -1;
    int msg_len;
#if 0
    if (is_subscribed == 0) {
        /* Subscribe the specific topic */
        rc = mqtt_subscribe(TOPIC_GET, mqtt_sub_callback, NULL);
        if (rc < 0) {
            // IOT_MQTT_Destroy(&pclient);
            LOG("IOT_MQTT_Subscribe() failed, rc = %d", rc);
        }
        is_subscribed = 1;
        //aos_schedule_call(ota_init, NULL);
    }
#endif

    if (msg->cmd==UDATA_MSG_REPORT_PUBLISH) {
        int ret = 0;
        ret = uData_report_publish(msg, &buf);
        if (ret != 0) {
            return;
        }
        integer_data_t *m_data =(integer_data_t*)buf.payload;
        switch (buf.type) {
            
            case UDATA_SERVICE_PH: {                
                msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"type\":\"PH\",\"vdata\":%d}", m_data->data);
                if (msg_len < 0) {
                    LOG("Error occur! Exit program");
                }
                break;
            }


            case UDATA_SERVICE_VWC: {
                msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"type\":\"VWC\",\"vdata\":%d}", m_data->data);
                if (msg_len < 0) {
                    LOG("Error occur! Exit program");
                }
                break;
            }

            case UDATA_SERVICE_EC: {
                msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"type\":\"EC\",\"vdata\":%d}", m_data->data);
                if (msg_len < 0) {
                    LOG("Error occur! Exit program");
                }
                break;
            }

            case UDATA_SERVICE_SALINITY: {
                msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"type\":\"SANILITY\",\"vdata\":%d}", m_data->data);
                if (msg_len < 0) {
                    LOG("Error occur! Exit program");
                }
                break;
            }

            case UDATA_SERVICE_TDS: {
                msg_len = snprintf(msg_pub, sizeof(msg_pub),  "{\"type\":\"TDS\",\"vdata\":%d}", m_data->data);
                if (msg_len < 0) {
                    LOG("Error occur! Exit program");
                }
                break;
            }

            case UDATA_SERVICE_HUMI: {
                msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"type\":\"HUMI\",\"vdata\":%d}", m_data->data);
                if (msg_len < 0) {
                    LOG("Error occur! Exit program");
                }
                break;
            }

            case UDATA_SERVICE_TEMP: {
                msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"type\":\"TEMP\",\"vdata\":%d}", m_data->data);
                if (msg_len < 0) {
                    LOG("Error occur! Exit program");
                }
                break;
            }

            default:
                break;

        }
        
        LOG("uData_report_publish2 type=%d,%s\n",buf.type,msg_pub);
        rc = mqtt_publish(TOPIC_DATA, IOTX_MQTT_QOS1, msg_pub, msg_len);
        if (rc < 0) {
            LOG("error occur when publish");
        }

    }
}
#endif

static aos_timer_t g_data_timer;
static bool            is_timer_work = false;

static void data_timer_process(void *arg)
{
    LOG("yftest %s:%d %s\r\n", __func__, __LINE__, aos_task_name());
    g_pub = 1;
    
}


int udata_sample(void)
{
    int ret = 0;

    uData_register_msg_handler(uData_report_demo);


     ret = uData_subscribe(UDATA_SERVICE_WINDSPD);
     if (ret != 0) {
         LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
     }
     ret = uData_subscribe(UDATA_SERVICE_WINDDIR);
      if (ret != 0) {
          LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
      }
      ret = uData_subscribe(UDATA_SERVICE_RAIN);
      if (ret != 0) {
          LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
      }
      ret = uData_subscribe(UDATA_SERVICE_TODAYRAIN);
      if (ret != 0) {
          LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
      }
    if(is_timer_work == false){
        
    aos_timer_new_ext(&g_data_timer, data_timer_process, NULL,
                            1000, 1, 1);
    aos_timer_start(&g_data_timer);
    is_timer_work = true;
        }
    return 0;
}

//static void ota_init(void *pclient);
int mqtt_client_example(void);
static void wifi_service_event(input_event_t *event, void *priv_data)
{
    if (event->type != EV_WIFI) {
        return;
    }

    if (event->code != CODE_WIFI_ON_GOT_IP) {
        return;
    }
    LOG("wifi_service_event!");
    mqtt_client_example();
}




/*
 * Subscribe the topic: IOT_MQTT_Subscribe(pclient, TOPIC_DATA, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
 * Publish the topic: IOT_MQTT_Publish(pclient, TOPIC_DATA, &topic_msg);
 */
static void mqtt_work(void *parms)
{

    int rc = -1;

    if (is_subscribed == 0) {
        /* Subscribe the specific topic */
        rc = mqtt_subscribe(TOPIC_GET, mqtt_sub_callback, NULL);
        if (rc < 0) {
            // IOT_MQTT_Destroy(&pclient);
            LOG("IOT_MQTT_Subscribe() failed, rc = %d", rc);
        }
        is_subscribed = 1;
        //aos_schedule_call(ota_init, NULL);
    }
#ifndef MQTT_PRESS_TEST
    else {
        /* Generate topic message */
        int msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"attr_name\":\"temperature\", \"attr_value\":\"%d\"}", cnt);
        if (msg_len < 0) {
            LOG("Error occur! Exit program");
        }
        rc = mqtt_publish(TOPIC_UPDATE, IOTX_MQTT_QOS1, msg_pub, msg_len);
        if (rc < 0) {
            LOG("error occur when publish");
        }

        LOG("packet-id=%u, publish topic msg=%s", (uint32_t)rc, msg_pub);
    }
    cnt++;
    if (cnt < 200) {
        aos_post_delayed_action(3000, mqtt_work, NULL);
    } else {
        aos_cancel_delayed_action(3000, mqtt_work, NULL);
        mqtt_unsubscribe(TOPIC_GET);
        aos_msleep(200);
        mqtt_deinit_instance();
        is_subscribed = 0;
        cnt = 0;
    }
#endif
}



static void mqtt_service_event(input_event_t *event, void *priv_data)
{

    if (event->type != EV_SYS) {
        return;
    }

    if (event->code != CODE_SYS_ON_MQTT_READ) {
        return;
    }
    LOG("mqtt_service_event!");
    udata_sample();
    //mqtt_work(NULL);
}

static int smartled_event_handler(int event_type, void *ctx)
{
    LOG("event_type %d\n", event_type);
    switch (event_type) {
        default:
            break;
    }

    return 0;
}

static MqttContext mqtt;

int mqtt_client_example(void)
{
    memset(&mqtt, 0, sizeof(MqttContext));

    strncpy(mqtt.productKey,   PRODUCT_KEY,   sizeof(mqtt.productKey)   - 1);
    strncpy(mqtt.deviceName,   DEVICE_NAME,   sizeof(mqtt.deviceName)   - 1);
    strncpy(mqtt.deviceSecret, DEVICE_SECRET, sizeof(mqtt.deviceSecret) - 1);

    mqtt.max_msg_size = MSG_LEN_MAX;
    mqtt.max_msgq_size = 8;

    mqtt.event_handler = smartled_event_handler;
    mqtt.delete_subdev = NULL;
    if (mqtt_init_instance(mqtt.productKey, mqtt.deviceName, mqtt.deviceSecret, mqtt.max_msg_size) < 0) {
        LOG("mqtt_init_instance failed\n");
        return -1;
    }
    aos_register_event_filter(EV_SYS,  mqtt_service_event, NULL);

    return 0;

}

static void handle_mqtt(char *pwbuf, int blen, int argc, char **argv)
{
    mqtt_client_example();
}

static struct cli_command mqttcmd = {
    .name = "mqtt",
    .help = "factory mqtt",
    .function = handle_mqtt
};

#ifdef AOS_ATCMD
static void at_uart_configure(uart_dev_t *u)
{
    u->port                = AT_UART_PORT;
    u->config.baud_rate    = AT_UART_BAUDRATE;
    u->config.data_width   = AT_UART_DATA_WIDTH;
    u->config.parity       = AT_UART_PARITY;
    u->config.stop_bits    = AT_UART_STOP_BITS;
    u->config.flow_control = AT_UART_FLOW_CONTROL;
}
#endif

int application_start(int argc, char *argv[])
{
#if AOS_ATCMD
    at.set_mode(ASYN);
    at.init(AT_RECV_PREFIX, AT_RECV_SUCCESS_POSTFIX,
            AT_RECV_FAIL_POSTFIX, AT_SEND_DELIMITER, 1000);
#endif


#ifdef WITH_SAL
    sal_init();
#elif defined (CSP_LINUXHOST)
    aos_post_event(EV_WIFI, CODE_WIFI_ON_GOT_IP, 0u);
#endif

    aos_set_log_level(AOS_LL_DEBUG);

    aos_register_event_filter(EV_WIFI, wifi_service_event, NULL);

    netmgr_init();
    netmgr_start(false);
    

    aos_cli_register_command(&mqttcmd);
    

    aos_loop_run();
    return 0;
}







