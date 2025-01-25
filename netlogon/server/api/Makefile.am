AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/server/include \
	-I$(top_srcdir)/netlogon/include \
    -I$(top_srcdir)/lwadvapi/include/public \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

noinst_LTLIBRARIES = liblwnetserverapi.la

liblwnetserverapi_la_CPPFLAGS = $(AM_CPPFLAGS)

liblwnetserverapi_la_SOURCES = \
       globals.c          \
       lwnet-server-cfg.c \
       lwnet-krb5.c       \
       lwnet-cachedb.c    \
       lwnet-netbios.c    \
       lwnet-plugin.c     \
       dcinfo.c           \
       event.c            \
       lwnet.c            \
       state.c

liblwnetserverapi_la_LIBADD = \
    $(top_builddir)/lwbase/src/liblwbase.la \
    $(top_builddir)/lwmsg/src/liblwmsg.la  \
	$(top_builddir)/lwreg/shellutil/librsutils.la \
    $(top_builddir)/lwadvapi/threaded/liblwadvapi_threaded.la \
	@PTHREAD_LIBS@

liblwnetserverapi_la_LDFLAGS =

noinst_HEADERS = \
		eventdlapi.h           \
		event_p.h              \
		evtstruct.h            \
		lwnet-cachedb.h        \
		lwnet-krb5_p.h         \
		lwnet_p.h              \
		lwnet-server-cfg_p.h   \
		state_p.h
