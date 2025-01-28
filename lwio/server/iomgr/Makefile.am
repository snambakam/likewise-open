if ENABLE_STATIC_DRIVERS
noinst_LTLIBRARIES = libiomgr.la
else
lib_LTLIBRARIES = libiomgr.la
endif

libiomgr_la_SOURCES = \
    ioinit.c \
    ioroot.c \
    iodriver.c \
    iodevice.c \
    ioirp.c \
    iofile.c \
    iomem.c \
    ioapi.c \
    ioipc.c \
    iosecurity.c \
    lwthreads.c \
    iolog.c \
    lwzct.c

libiomgr_la_CPPFLAGS = \
    -DLW_ENABLE_THREADS=1 \
    -I@top_srcdir@/lwio/include \
    -I@top_srcdir@/lwio/server/include \
    -I@top_srcdir@/lwreg/include \
    -I@top_srcdir@/lwmsg/include \
    -I@top_srcdir@/lwbase/include

libiomgr_la_CFLAGS = \
    $(AM_CFLAGS) \
    -Wno-unused-but-set-variable

libiomgr_la_LIBADD = \
    @top_builddir@/lwio/common/liblwiocommon.la \
    @top_builddir@/lwio/logging_r/liblwiolog_r.la  \
    @top_builddir@/lwbase/src/liblwbase.la \
    @top_builddir@/lwmsg/src/liblwmsg.la \
    @top_builddir@/lwreg/client/libregclient.la \
    @UUID_LIBS@  \
    @PTHREAD_LIBS@ \
    @RT_LIBS@ \
    @SOCKET_LIBS@

libiomgr_la_LDFLAGS = \
    -no-undefined
