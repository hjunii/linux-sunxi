# Configuration for A10

ifdef CONFIG_ARCH_SUN7I
CONFIG:=ca7-virtex820-m400-2
MALI_PLATFORM_FILE:=platform/${CONFIG}/mali_platform.c
export MALI_PLATFORM_FILE
else
CONFIG:=ca8-virtex820-m400-1
endif

USING_MMU:=1
USING_UMP:=1
USING_PMM:=1
BUILD:=release
TARGET_PLATFORM:=arm
ifneq ($(CONFIG_TRACEPOINTS),y)
USING_PROFILING:=0
export USING_PROFILING
endif
export CONFIG USING_MMU USING_UMP USING_PMM BUILD TARGET_PLATFORM

export EXTRA_DEFINES = -DMALI_FAKE_PLATFORM_DEVICE=1
export MALI_PLATFORM_FILES = platform/arm/arm.c platform/arm/arm_core_scaling.c

obj-$(CONFIG_MALI) += devicedrv/ump/ devicedrv/mali/
