QT += qml quick quickcontrols2 widgets multimedia svg

CONFIG += c++11

android:{
QT+= androidextras remoteobjects

REPC_REPLICA += qmdxplayer.rep
REPC_REPLICA += playlistmanager.rep
REPC_SOURCE += qmdxplayer.rep
REPC_SOURCE += playlistmanager.rep
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DEFINES += ALWAYSFULLSCREEN
SOURCES +=  \
    playerservice.cpp \
    qmdxplayerclientproxy.cpp \
    qmdxplayerserviceproxy.cpp \
    playlistmanagerserviceproxy.cpp \
    playlistmanagerclientproxy.cpp

HEADERS += \
    playerservice.h \
    qmdxplayerclientproxy.h \
    qmdxplayerserviceproxy.h \
    playlistmanagerserviceproxy.h \
    playlistmanagerclientproxy.h
}

win32:{
RC_ICON = icon/XMDX.ico
}

SOURCES += main.cpp \
    mdx2wav/gamdx/downsample/downsample.cpp \
    mdx2wav/gamdx/fmgen/fmgen.cpp \
    mdx2wav/gamdx/fmgen/fmtimer.cpp \
    mdx2wav/gamdx/fmgen/opm.cpp \
    mdx2wav/gamdx/mxdrvg/opm_delegate.cpp \
    mdx2wav/gamdx/mxdrvg/so.cpp \
    mdx2wav/gamdx/pcm8/pcm8.cpp \
    mdx2wav/gamdx/pcm8/x68pcm8.cpp \
    mdx2wav/src/mdx2wav.cpp \
    mdx2wav/gamdx/mame/ym2151.c \
    qmdxplayer.cpp \
    playlistitem.cpp \
    playlistmanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    mdx2wav/gamdx/downsample/lowpass_44.dat \
    mdx2wav/gamdx/downsample/lowpass_48.dat \
    mdx2wav/gamdx/fmgen/readme.txt \
    mdx2wav/LICENSE \
    icon/openiconic/loop.svg \
    icon/openiconic/media-pause.svg \
    icon/openiconic/media-play.svg \
    icon/openiconic/media-skip-backward.svg \
    icon/openiconic/media-skip-forward.svg \
    icon/openiconic/media-step-backward.svg \
    icon/openiconic/media-step-forward.svg \
    icon/openiconic/media-stop.svg \
    icon/openiconic/menu.svg \
    icon/openiconic/random.svg \
    icon/openiconic/volume-high.svg \
    icon/openiconic/volume-low.svg \
    icon/openiconic/x.svg \
    icon/openiconic/ICON-LICENSE \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    icon/XMDX.png \
    icon/XMDX.svg \
    icon/XMDX.ico \
    android/res/drawable/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/src/org/eighttails/xmdx/PlayerService.java \
    qmdxplayer.rep \
    playlistmanager.rep \
    design/ClassDiagram.png \
    design/PlayerSequence.png \
    design/ClassDiagram.puml \
    design/PlayerSequence.puml \
    README.md

HEADERS += \
    mdx2wav/gamdx/downsample/downsample.h \
    mdx2wav/gamdx/downsample/global.h \
    mdx2wav/gamdx/fmgen/diag.h \
    mdx2wav/gamdx/fmgen/fmgen.h \
    mdx2wav/gamdx/fmgen/fmgeninl.h \
    mdx2wav/gamdx/fmgen/fmtimer.h \
    mdx2wav/gamdx/fmgen/headers.h \
    mdx2wav/gamdx/fmgen/misc.h \
    mdx2wav/gamdx/fmgen/opm.h \
    mdx2wav/gamdx/mame/ym2151.h \
    mdx2wav/gamdx/mxdrvg/mxdrvg.h \
    mdx2wav/gamdx/mxdrvg/mxdrvg_core.h \
    mdx2wav/gamdx/mxdrvg/mxdrvg_depend.h \
    mdx2wav/gamdx/mxdrvg/opm_delegate.h \
    mdx2wav/gamdx/pcm8/global.h \
    mdx2wav/gamdx/pcm8/pcm8.h \
    mdx2wav/gamdx/pcm8/x68pcm8.h \
    mdx2wav/gamdx/types.h \
    qmdxplayer.h \
    playlistitem.h \
    playlistmanager.h
