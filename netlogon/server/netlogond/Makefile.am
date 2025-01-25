AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/server/include \
	-I$(top_srcdir)/netlogon/include \
    -I$(top_srcdir)/lwadvapi/include/public \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

sbin_PROGRAMS = netlogond

netlogond_CPPFLAGS = $(AM_CPPFLAGS)

netlogond_LDADD = \
	@top_builddir@/lwbase/src/liblwbase.la \
	@top_builddir@/lwadvapi/threaded/liblwadvapi_threaded.la \
	@top_builddir@/lwadvapi/threaded/liblwadvapi_threaded.la \
	@top_builddir@/lwadvapi/unthreaded/liblwadvapi_nothr.la \
	@top_builddir@/lwreg/shellutil/librsutils.la \
	@top_builddir@/netlogon/server/ipc/liblwnetserveripc.la \
	@top_builddir@/netlogon/server/api/liblwnetserverapi.la \
	@top_builddir@/netlogon/common/liblwnetcommon.la \
	@RT_LIBS@ \
	@DS_FRAMEWORK_LDFLAGS@

netlogond_SOURCES = \
	globals.c    \
	listener.c   \
	main.c       \
	sighandler.c
