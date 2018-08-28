src = []
component = aos_component('mbmaster', src)
component.add_global_includes('include')
component.add_global_macros('CONFIG_AOS_MBMASTER')

libpath = '/'.join(['lib', "Cortex-M4", "gcc", 'mbmaster.a'])
component.add_prebuilt_libs(libpath)