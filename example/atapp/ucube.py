src =Split(''' 
    atapp.c
''')
component =aos_component('atapp', src)

dependencis =Split(''' 
    kernel/yloop
    tools/cli
    framework/netmgr
    framework/atparser
''')
for i in dependencis:
    component.add_comp_deps(i)

component.add_global_macros('AOS_NO_WIFI')




