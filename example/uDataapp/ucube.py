src =Split(''' 
    uData-example.c
    uData_app.c
    linkkit_export.c
    lite_queue.c
    uData_tsl.c
''')
component =aos_component('uDataapp', src)
	
dependencis =Split(''' 
    framework/connectivity/mqtt
    framework/fota
    framework/netmgr
    framework/common
    framework/protocol/linkkit/cm
    framework/protocol/linkkit/dm
    framework/protocol/linkkit/alcs
    framework/protocol/linkkit/iotkit
    framework/ywss4linkkit
    tools/cli
    device/sensor 
    framework/uData
''')
for i in dependencis:
    component.add_comp_deps(i)

global_macros =Split(''' 
    MQTT_DIRECT
    ALIOT_DEBUG
    IOTX_DEBUG
    USE_LPTHREAD
    FOTA_RAM_LIMIT_MODE
    COAP_WITH_YLOOP
    TEST_ALCS
    CONFIG_AOS_CLI
    ON_PRE2=1
    DATA_TO_CLOUD
''')
for i in global_macros:
    component.add_global_macros(i)

if aos_global_config.get('LWIP') == 1:
    component.add_comp_deps("kernel/protocols/net")
    aos_global_config.set('no_with_lwip', 0)

