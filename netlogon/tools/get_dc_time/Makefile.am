AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/include \
    -I$(top_srcdir)/lwadvapi/include/public \
	-I$(top_srcdir)/lwreg/include \
	-I$(top_srcdir)/lwmsg/include \
	-I$(top_srcdir)/lwbase/include

bin_PROGRAMS = lw-get-dc-time

lw_get_dc_time_LDADD = \
	@top_builddir@/lwbase/src/liblwbase_nothr.la \
	@top_builddir@/lwadvapi/threaded/liblwadvapi_threaded.la \
	@top_builddir@/lwadvapi/unthreaded/liblwadvapi_nothr.la \
	@top_builddir@/netlogon/client/liblwnetclientapi.la \
	@top_builddir@/netlogon/common/liblwnetcommon.la

lw_get_dc_time_SOURCES = main.c
