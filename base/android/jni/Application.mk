APP_ABI := all
# APP_ABI := armeabi-v7a x86
# APP_ABI := armeabi armeabi-v7a mips x86
# APP_ABI := x86
# APP_STL := gnustl_static
# APP_STL := stlport_static
# APP_STL := c++_static
# NDK_TOOLCHAIN_VERSION := clang
# APP_CPPFLAGS += -std=c++14

APP_STL := c++_shared
NDK_TOOLCHAIN_VERSION := clang
APP_CPPFLAGS += -frtti -fexceptions
