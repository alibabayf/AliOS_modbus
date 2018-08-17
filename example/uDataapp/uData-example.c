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

#ifdef DATA_TO_CLOUD

static int linkkit_started = 0;
static int awss_running = 0;
extern int linkkit_app(void);
void reboot_system(void *parms);

int awss_success_notify();
static void wifi_service_event(input_event_t *event, void *priv_data)
{
    if (event->type != EV_WIFI) {
        return;
    }

    if (event->code != CODE_WIFI_ON_GOT_IP) {
        return;
    }

    netmgr_ap_config_t config;
    memset(&config, 0, sizeof(netmgr_ap_config_t));
    netmgr_get_ap_config(&config);
    LOG("wifi_service_event config.ssid %s", config.ssid);
    if (strcmp(config.ssid, "adha") == 0 || strcmp(config.ssid, "aha") == 0) {
        //clear_wifi_ssid();
        return;
    }

    if (awss_running) {
#ifdef AWSS_NEED_REBOOT
        aos_post_delayed_action(200, reboot_system, NULL);
#endif
        return;
    }
    if (!linkkit_started) {
        linkkit_app();
        awss_success_notify();
        linkkit_started = 1;
    }
}

void reboot_system(void *parms)
{
    LOG("reboot system");
    aos_reboot();
}

static void cloud_service_event(input_event_t *event, void *priv_data)
{
    static uint8_t awss_reported = 0;
    if (event->type != EV_YUNIO) {
        return;
    }

    LOG("cloud_service_event %d", event->code);

    if (event->code == CODE_YUNIO_ON_CONNECTED) {
        LOG("user sub and pub here");
        if (!awss_reported) {
            awss_report_cloud();
            awss_reported = 1;
        }
        return;
    }

    if (event->code == CODE_YUNIO_ON_DISCONNECTED) {
    }
}

static void start_netmgr(void *p)
{
    netmgr_start(true);
    //aos_task_exit(0);
}

extern int awss_report_reset();

void do_awss_active()
{
    LOG("do_awss_active %d\n", awss_running);
    awss_running = 1;
    awss_config_press();
}

static void do_awss_reset()
{
    if (linkkit_started) {
        aos_task_new("reset", awss_report_reset, NULL, 2048);
    }
    netmgr_clear_ap_config();
    LOG("SSID cleared. Please reboot the system.\n");
    aos_post_delayed_action(1000, reboot_system, NULL);
}

void linkkit_key_process(input_event_t *eventinfo, void *priv_data)
{
    if (eventinfo->type != EV_KEY) {
        return;
    }
    LOG("awss config press %d\n", eventinfo->value);

    if (eventinfo->code == CODE_BOOT) {
        if (eventinfo->value == VALUE_KEY_CLICK) {
            do_awss_active();
        } else if (eventinfo->value == VALUE_KEY_LTCLICK) {
            do_awss_reset();
        }
    }
}

#ifdef CONFIG_AOS_CLI
static void handle_reset_cmd(char *pwbuf, int blen, int argc, char **argv)
{
    aos_schedule_call(do_awss_reset, NULL);
}

static void handle_active_cmd(char *pwbuf, int blen, int argc, char **argv)
{
    aos_schedule_call(do_awss_active, NULL);
}

static struct cli_command resetcmd = {
    .name = "reset",
    .help = "factory reset",
    .function = handle_reset_cmd
};

static struct cli_command ncmd = {
    .name = "active_awss",
    .help = "active_awss [start]",
    .function = handle_active_cmd
};
#endif
#endif


void uData_report_demo(input_event_t *event, void *priv_data)
{
    udata_pkg_t buf;
    if ((event == NULL) || (event->type != EV_UDATA)) {
        return;
    }

    if (event->code == CODE_UDATA_REPORT_PUBLISH) {
        int ret = 0;
        ret = uData_report_publish(event, &buf);
        if (ret != 0) {
            return;
        }
        switch (buf.type) {

            case UDATA_SERVICE_ACC: {
                accel_data_t *acc = (accel_data_t *)buf.payload;
                UDATA_SHOW_UINT_3(buf.type, (uint32_t)acc->timestamp, acc->data[0], acc->data[1], acc->data[2]);
                break;
            }


            case UDATA_SERVICE_MAG: {
                mag_data_t *mag = (mag_data_t *)buf.payload;
                UDATA_SHOW_UINT_3(buf.type, (uint32_t)mag->timestamp, mag->data[0], mag->data[1], mag->data[2]);
                break;
            }

            case UDATA_SERVICE_GYRO: {
                gyro_data_t *gyro = (gyro_data_t *)buf.payload;
                UDATA_SHOW_UINT_3(buf.type, (uint32_t)gyro->timestamp, gyro->data[0], gyro->data[1], gyro->data[2]);
                break;
            }

            case UDATA_SERVICE_ALS: {
                als_data_t *als = (als_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, als->timestamp, als->lux);
                break;
            }

            case UDATA_SERVICE_PS: {
                proximity_data_t *ps = (proximity_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, ps->timestamp, ps->present);
                break;
            }

            case UDATA_SERVICE_BARO: {
                barometer_data_t *baro = (barometer_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, baro->timestamp, baro->p);
                break;
            }

            case UDATA_SERVICE_TEMP: {
                temperature_data_t *temp = (temperature_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, temp->timestamp, temp->t);
                break;
            }

            case UDATA_SERVICE_UV: {
                uv_data_t *uv = (uv_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, uv->timestamp, uv->uvi);
                break;
            }

            case UDATA_SERVICE_HUMI: {
                humidity_data_t *humi = (humidity_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, humi->timestamp, humi->h);
                break;
            }

            case UDATA_SERVICE_HALL: {
                hall_data_t *hall = (hall_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, hall->timestamp, hall->hall_level);
                break;
            }
            case UDATA_SERVICE_HR: {
                heart_rate_data_t *heart = (heart_rate_data_t *)buf.payload;
                UDATA_SHOW_UINT_1(buf.type, heart->timestamp, heart->hear_rate);
                break;
            }

            case UDATA_SERVICE_GPS: {
                gps_data_t *gps = (gps_data_t *)buf.payload;
                UDATA_SHOW_FLOAT_3(buf.type, (uint32_t)gps->timestamp, gps->lat, gps->lon, gps->elv);
                break;
            }


            default:
                break;

        }

    }
}

int udata_sample(void)
{
    int ret = 0;

    aos_register_event_filter(EV_UDATA, uData_report_demo, NULL);

    ret = uData_subscribe(UDATA_SERVICE_BARO);
     ret = uData_subscribe(UDATA_SERVICE_PH);
     if (ret != 0) {
         LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
     }
     ret = uData_subscribe(UDATA_SERVICE_TEMP);
     if (ret != 0) {
         LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
     }
    ret = uData_subscribe(UDATA_SERVICE_HUMI);
     if (ret != 0) {
         LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
     }
     ret = uData_subscribe(UDATA_SERVICE_VWC);
      if (ret != 0) {
          LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
      }
     ret = uData_subscribe(UDATA_SERVICE_EC);
      if (ret != 0) {
          LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
      }
      ret = uData_subscribe(UDATA_SERVICE_SALINITY);
      if (ret != 0) {
          LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
      }
      ret = uData_subscribe(UDATA_SERVICE_TDS);
      if (ret != 0) {
          LOG("%s %s %s %d\n", uDATA_STR, __func__, ERROR_LINE, __LINE__);
      }


    return 0;
}



int application_start(int argc, char **argv)
{
#ifdef DATA_TO_CLOUD
#ifdef CSP_LINUXHOST
    signal(SIGPIPE, SIG_IGN);
#endif
#if AOS_ATCMD
    at.set_mode(ASYN);
    at.init(AT_RECV_PREFIX, AT_RECV_SUCCESS_POSTFIX,
            AT_RECV_FAIL_POSTFIX, AT_SEND_DELIMITER, 1000);
#endif


#ifdef WITH_SAL
    sal_init();
#endif
    aos_set_log_level(AOS_LL_DEBUG);

    netmgr_init();
    aos_register_event_filter(EV_KEY, linkkit_key_process, NULL);
    aos_register_event_filter(EV_WIFI, wifi_service_event, NULL);
    aos_register_event_filter(EV_YUNIO, cloud_service_event, NULL);

#ifdef CONFIG_AOS_CLI
    aos_cli_register_command(&resetcmd);
    aos_cli_register_command(&ncmd);
#endif
    aos_task_new("netmgr", start_netmgr, NULL, 4096);

#endif
    udata_sample();


    aos_loop_run();

    return 0;
}
