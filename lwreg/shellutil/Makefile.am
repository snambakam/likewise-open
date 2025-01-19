lib_LTLIBRARIES = librsutils.la

librsutils_la_SOURCES = \
    regutilwrap.c \
    rsutils.c

librsutils_la_CPPFLAGS = \
    $(AM_CPPFLAGS)      \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

librsutils_la_LIBADD = \
    $(top_builddir)/lwreg/client/libregclient.la \
    -llwbase_nothr

librsutils_la_LDFLAGS = \
    -L$(top_builddir)/lwbase/src
