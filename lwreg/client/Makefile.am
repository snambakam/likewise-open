lib_LTLIBRARIES = libregclient.la

libregclient_la_SOURCES = \
    clientipc.c \
    regclient.c \
    regntclient.c

libregclient_la_CPPFLAGS = \
    $(AM_CPPFLAGS) \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

libregclient_la_LIBADD = \
	@top_builddir@/lwreg/common/libregcommon.la
