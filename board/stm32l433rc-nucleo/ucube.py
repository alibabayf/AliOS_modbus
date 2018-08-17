src     = ['aos/board_partition.c', 'aos/soc_init.c', 'Src/stm32l4xx_hal_msp.c']

component = aos_board_component('stm32l433rc-nucleo', 'stm32l4xx_cube', src)

if aos_global_config.compiler == "armcc":
    component.add_sources('startup_stm32l433xx_keil.s')
    component.add_prebuilt_objs('startup_stm32l433xx_keil.o', 'Src/stm32l4xx_hal_msp.o')
elif aos_global_config.compiler == "iar":
    component.add_sources('startup_stm32l433xx_iar.s')
else:
    component.add_sources('startup_stm32l433xx.s')
    aos_global_config.add_ld_files('STM32L433RCTxP_FLASH.ld')

component.add_global_includes('hal', 'aos', 'Inc')
component.add_global_macros('STM32L433xx', 'SYSINFO_PRODUCT_MODEL=\\"ALI_AOS_432-nucleo\\"', 'SYSINFO_DEVICE_NAME=\\"432-nucleo\\"')

if aos_global_config.get('sal', 1) == 1:
    component.add_comp_deps('device/sal')
    module = aos_global_config.get('module', 'wifi.mk3060')
    if module == 'wifi.mk3060':
        aos_global_config.set('module', 'wifi.mk3060')

component.set_enable_vfp()

