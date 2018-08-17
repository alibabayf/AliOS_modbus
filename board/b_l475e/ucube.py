src     = Split('''
        board.c
        osa_flash.c
''')


component = aos_board_component('board_b_l475e', 'stm32l475', src)

if aos_global_config.get('sal') == None:
    aos_global_config.set('sal',1)
if aos_global_config.get('sal') == 1:
    component.add_comp_deps('device/sal')
    if aos_global_config.get('module') == None:
        aos_global_config.set('module','wifi.mk3060')
else:
    component.add_global_macros('CONFIG_NO_TCPIP')

component.add_global_includes('.')

component.add_global_macros('STDIO_UART=0')
component.add_global_macros('RHINO_CONFIG_TICK_TASK=0')
component.add_global_macros('STM32L475xx')

CONFIG_SYSINFO_PRODUCT_MODEL = 'ALI_AOS_B-L475E'
CONFIG_SYSINFO_DEVICE_NAME = 'B-L475E'
aos_global_config.set('CONFIG_SYSINFO_PRODUCT_MODEL',CONFIG_SYSINFO_PRODUCT_MODEL)
aos_global_config.set('CONFIG_SYSINFO_DEVICE_NAME',CONFIG_SYSINFO_DEVICE_NAME)

component.add_global_macros('SYSINFO_PRODUCT_MODEL=\\"'+CONFIG_SYSINFO_PRODUCT_MODEL+'\\"')
component.add_global_macros('SYSINFO_DEVICE_NAME=\\"'+CONFIG_SYSINFO_DEVICE_NAME+'\\"')
component.add_global_macros('RHINO_CONFIG_WORKQUEUE=0')

supported_targets="nano helloworld mqttapp tls uDataapp networkapp acapp"
windows_only_targets="nano|COMPILER=armcc helloworld|COMPILER=armcc mqttapp|COMPILER=armcc acapp|COMPILER=armcc nano|COMPILER=iar helloworld|COMPILER=iar mqttapp|COMPILER=iar acapp|COMPILER=iar mqttapp|IDE=keil mqttapp|IDE=iar"

