lib_LTLIBRARIES = liblwioshareinfo.la

liblwioshareinfo_la_SOURCES = \
    libmain.c                 \
    lwfileinfo.c              \
    lwsessioninfo.c           \
    lwshareinfo.c             \
    lwconnectioninfo.c

liblwioshareinfo_la_CPPFLAGS = \
    $(AM_CPPFLAGS) \
    -I@top_srcdir@/lwio/include \
    -I@top_srcdir@/lwmsg/include \
    -I@top_srcdir@/lwbase/include

liblwioshareinfo_la_CFLAGS = \
    $(AM_CFLAGS)
