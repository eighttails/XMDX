QT += qml quick multimedia quickcontrols2

CONFIG += c++11

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
    helper.cpp \
    playlistitem.cpp

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
    icon/loop.svg \
    icon/media-pause.svg \
    icon/media-play.svg \
    icon/media-skip-backward.svg \
    icon/media-skip-forward.svg \
    icon/media-step-backward.svg \
    icon/media-step-forward.svg \
    icon/media-stop.svg \
    icon/menu.svg \
    icon/random.svg \
    icon/volume-high.svg \
    icon/volume-low.svg \
    icon/x.svg \
    icon/ICON-LICENSE

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
    helper.h \
    playlistitem.h
