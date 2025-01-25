AM_CPPFLAGS = \
    -I$(top_srcdir)/netlogon/include \
    -I$(top_srcdir)/lwadvapi/include/public \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

lib_LTLIBRARIES = liblwnetclientapi.la

liblwnetclientapi_la_CPPFLAGS = $(AM_CPPFLAGS)

if ENABLE_MINIMAL
liblwnetclientapi_la_CPPFLAGS += "-DMINIMAL_NETLOGON=1"
endif

liblwnetclientapi_la_LIBADD =   \
    $(top_builddir)/lwmsg/src/liblwmsg.la  \
    $(top_builddir)/lwadvapi/unthreaded/liblwadvapi_nothr.la \
    $(top_builddir)/netlogon/common/liblwnetcommon.la

liblwnetclientapi_la_LDFLAGS =

liblwnetclientapi_la_SOURCES = \
    dcinfo.c     \
    ipc_client.c \
    krb5env.c
