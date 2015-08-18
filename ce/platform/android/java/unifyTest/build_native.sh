#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"

echo "- config:"
echo "  ANDROID_NDK_ROOT    = $ANDROID_NDK_ROOT"
echo "  UNIFY_ROOT          = $UNIFY_ROOT"
echo "  APP_ROOT            = $APP_ROOT"
echo "  APP_ANDROID_ROOT    = $APP_ANDROID_ROOT"

# if dont use DEBUG, comments out two lines below
NDK_DEBUG=1
NDK_BUILD_FLAGS="CPPFLAGS=\"-DCE_DEBUG=1\""

echo "- cleanup"
find "$APP_ANDROID_ROOT" -type d | xargs chmod 755 $1
if [ -d "$APP_ANDROID_ROOT"/bin ]; then
    rm -rf "$APP_ANDROID_ROOT"/bin/*.apk
fi
mkdir -p "$APP_ANDROID_ROOT"/bin
chmod 755 "$APP_ANDROID_ROOT"/bin


# build
echo "Using prebuilt externals"
"$ANDROID_NDK_ROOT"/ndk-build $ANDROID_NDK_BUILD_FLAGS NDK_DEBUG=$NDK_DEBUG $NDK_BUILD_FLAGS -C "$APP_ANDROID_ROOT" $* "NDK_MODULE_PATH=${UNIFY_ROOT}"