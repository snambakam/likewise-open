SUBDIRS = .

noinst_LTLIBRARIES = liblwioipc.la

liblwioipc_la_SOURCES = \
    globals.c \
    libmain.c \
    ntipc.c \
    ntipc-unicode_string.c

liblwioipc_la_CPPFLAGS = \
    -I@top_srcdir@/lwio/include  \
    -I@top_srcdir@/lwmsg/include  \
    -I@top_srcdir@/lwbase/include

liblwioipc_la_LIBADD = \
    @top_builddir@/lwmsg/src/liblwmsg_nothr.la