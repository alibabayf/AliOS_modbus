NAME := uData

$(NAME)_SOURCES += \
    uData_main.c \
    uData_interface.c \
    service_mgr/service_mgr.c \
    cali_data/calibrated_example_algo.c \
    abs_data_model/abs_data_model.c \
    service/service_process.c \
    uData_queue.c

ifeq ($(dtc),1)
$(NAME)_SOURCES += service/service_data_to_cloud.c
endif


$(NAME)_INCLUDES := \
    ./include \
    ../../include/aos \
	../../include/hal \
	
GLOBAL_INCLUDES += .

$(NAME)_TYPE := framework

#GLOBAL_DEFINES += UDATA_YLOOP
GLOBAL_DEFINES += AOS_UDATA

GLOBAL_DEFINES += AOS_UDATA_SERVICE_HUMI
GLOBAL_DEFINES += AOS_UDATA_SERVICE_TEMP
GLOBAL_DEFINES += AOS_UDATA_SERVICE_PH
GLOBAL_DEFINES += AOS_UDATA_SERVICE_VWC
GLOBAL_DEFINES += AOS_UDATA_SERVICE_EC
GLOBAL_DEFINES += AOS_UDATA_SERVICE_SALINITY
GLOBAL_DEFINES += AOS_UDATA_SERVICE_TDS