AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/server/api \
	-I$(top_srcdir)/netlogon/server/include \
	-I$(top_srcdir)/netlogon/include \
    -I$(top_srcdir)/lwadvapi/include/public \
	-I$(top_srcdir)/lwreg/include \
	-I$(top_srcdir)/lwmsg/include \
	-I$(top_srcdir)/lwbase/include

bin_PROGRAMS = lw-export-dc-cache

lw_export_dc_cache_CPPFLAGS = $(AM_CPPFLAGS)

lw_export_dc_cache_LDADD = \
	@top_builddir@/lwbase/src/liblwbase_nothr.la \
	@top_builddir@/lwadvapi/threaded/liblwadvapi_threaded.la \
	@top_builddir@/lwadvapi/unthreaded/liblwadvapi_nothr.la \
	@top_builddir@/netlogon/server/api/liblwnetserverapi.la \
	@top_builddir@/netlogon/common/liblwnetcommon.la

lw_export_dc_cache_SOURCES = main.c
