# TARGET #

TARGET := 3DS
LIBRARY := 0

ifeq ($(TARGET),$(filter $(TARGET),3DS WIIU))
    ifeq ($(strip $(DEVKITPRO)),)
        $(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
    endif
endif

# PORTLIBS #

export PORTLIBS_PATH	:=	$(DEVKITPRO)/portlibs
PORTLIBS	:=	$(PORTLIBS_PATH)/armv6k $(PORTLIBS_PATH)/3ds

# COMMON CONFIGURATION #

NAME := 3DSThemeManager

BUILD_DIR := build
OUTPUT_DIR := output
INCLUDE_DIRS := include
SOURCE_DIRS := source

EXTRA_OUTPUT_FILES :=

LIBRARY_DIRS := $(DEVKITPRO)/libctru $(PORTLIBS)
LIBRARIES := vorbisidec ogg sfil sftd sf2d freetype png z

BUILD_FLAGS := -DVERSION='"1.3.1-f1"'
RUN_FLAGS :=

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    LIBRARIES += citro3dd ctrud
    BUILD_FLAGS += -Og
else
    LIBRARIES += citro3d ctru
    BUILD_FLAGS += -O4
endif

# 3DS/Wii U CONFIGURATION #

ifeq ($(TARGET),$(filter $(TARGET),3DS WIIU))
    TITLE := 3DSThemeManager
    DESCRIPTION := A more beautiful theme manager
    AUTHOR := Favna
endif

# 3DS CONFIGURATION #

ifeq ($(TARGET),3DS)
    LIBRARY_DIRS += $(DEVKITPRO)/libctru
    LIBRARIES += ctru

    PRODUCT_CODE := CIA-F-3DTM
    UNIQUE_ID := 0x374E5

    CATEGORY := Application
    USE_ON_SD := true

    MEMORY_TYPE := Application
    SYSTEM_MODE := 64MB
    SYSTEM_MODE_EXT := 124MB
    CPU_SPEED := 804MHz
    ENABLE_L2_CACHE := true

    ICON_FLAGS :=

    ROMFS_DIR := romfs
    BANNER_AUDIO := meta/audio_3ds.wav
    BANNER_IMAGE := meta/banner_3ds.png
    ICON := meta/icon_3ds.png
    LOGO := 
endif

# INTERNAL #

include buildtools/make_base
