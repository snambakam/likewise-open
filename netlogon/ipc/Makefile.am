AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/include \
	-I$(top_srcdir)/lwadvapi/include/public \
	-I$(top_srcdir)/lwreg/include \
	-I$(top_srcdir)/lwmsg/include \
	-I$(top_srcdir)/lwbase/include

noinst_LTLIBRARIES = liblwnetipc.la

liblwnetipc_la_LIBADD = \
	$(top_builddir)/lwmsg/src/liblwmsg.la

liblwnetipc_la_SOURCES =   \
	ipc_protocol.c

noinst_headers = includes.h
