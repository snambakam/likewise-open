AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/server/include \
	-I$(top_srcdir)/netlogon/include \
    -I$(top_srcdir)/lwadvapi/include/public \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

noinst_LTLIBRARIES = liblwnetserveripc.la

liblwnetserveripc_la_CPPFLAGS = $(AM_CPPFLAGS)

liblwnetserveripc_la_SOURCES = \
	ipc_dcinfo.c    \
	globals.c

liblwnetserveripc_la_LIBADD = \
    $(top_builddir)/lwmsg/src/liblwmsg.la

noinst_HEADERS = \
    includes.h        \
    externs_p.h       \
    ipc_dcinfo_p.h
