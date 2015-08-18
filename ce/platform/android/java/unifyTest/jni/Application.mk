APP_STL := gnustl_static
#APP_CPPFLAGS := -D__GXX_EXPERIMENTAL_CXX0X__ -std=c++11 -frtti -Wno-error=format-security -fsigned-char -Os $(CPPFLAGS)

APP_CPPFLAGS := -frtti -std=c++11 -DCE_DEBUG=1
APP_ABI := armeabi

NDK_TOOLCHAIN_VERSION=clang3.4