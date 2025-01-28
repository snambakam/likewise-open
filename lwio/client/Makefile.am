lib_LTLIBRARIES = liblwioclient.la

liblwioclient_la_SOURCES = \
    access.c               \
    config.c               \
    connect.c              \
    driver.c               \
    globals.c              \
    libmain.c              \
    logging.c              \
    ntfileapictx.c         \
    smbfileapictx.c        \
    srvshareapi.c          \
    srvtransportapi.c      \
    thread.c               \
    win32.c

liblwioclient_la_CPPFLAGS = \
    $(AM_CPPFLAGS) \
    -DLWIO_NO_THREADS \
    -I@top_srcdir@/lwio/include \
    -I@top_srcdir@/lwmsg/include \
    -I@top_srcdir@/lwbase/include

liblwioclient_la_CFLAGS = \
    $(AM_CFLAGS) \
    -Wmissing-prototypes

liblwioclient_la_LIBADD = \
    @top_builddir@/lwio/common/liblwiocommon.la \
    @top_builddir@/lwio/libshareinfo/liblwioshareinfo.la \
    @KRB5_LIBS@
