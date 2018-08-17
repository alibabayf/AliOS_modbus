src =Split(''' 
    uData_main.c
    uData_interface.c
    service_mgr/service_mgr.c
    cali_data/calibrated_example_algo.c
    abs_data_model/abs_data_model.c
    service/service_process.c
    service/service_data_to_cloud.c
    uData_queue.c
''')
component =aos_component('uData', src)


global_includes =Split(''' 
    .
''')
for i in global_includes:
    component.add_global_includes(i)

global_macros =Split(''' 
    AOS_UDATA
    AOS_UDATA_SERVICE_HUMI
    AOS_UDATA_SERVICE_TEMP
    AOS_UDATA_SERVICE_PH
    AOS_UDATA_SERVICE_VWC
    AOS_UDATA_SERVICE_EC
    AOS_UDATA_SERVICE_SALINITY
    AOS_UDATA_SERVICE_TDS
''')
for i in global_macros:
    component.add_global_macros(i)

includes =Split(''' 
    ./include
    ../../include/aos
    ../../include/hal
''')
for i in includes:
    component.add_includes(i)


