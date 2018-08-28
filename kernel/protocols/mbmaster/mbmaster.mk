NAME := mbmaster

$(NAME)_MBINS_TYPE := kernel
GLOBAL_INCLUDES += include
GLOBAL_DEFINES += CONFIG_AOS_MBMASTER

$(NAME)_PREBUILT_LIBRARY := lib/Cortex-M4/gcc/mbmaster.a
