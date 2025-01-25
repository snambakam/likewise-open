plugindir = $(libdir)/krb5/plugins/libkrb5

plugin_LTLIBRARIES = liblwnet_service_locator.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/include \
	-I$(top_srcdir)/lwreg/include \
	-I$(top_srcdir)/lwmsg/include \
	-I$(top_srcdir)/lwadvapi/include/public \
	-I$(top_srcdir)/lwbase/include \
	$(KRB5_INCLUDES)

liblwnet_service_locator_la_CPPFLAGS = $(AM_CPPFLAGS)

liblwnet_service_locator_la_LDFLAGS = \
	-module \
	$(KRB5_LDFLAGS)

liblwnet_service_locator_la_SOURCES = service_locator.c

liblwnet_service_locator_la_LIBADD = \
	$(top_builddir)/netlogon/client/liblwnetclientapi.la \
	@top_builddir@/lwadvapi/threaded/liblwadvapi_threaded.la \
	@top_builddir@/lwadvapi/unthreaded/liblwadvapi_nothr.la \
	$(KRB5_LIBS)
