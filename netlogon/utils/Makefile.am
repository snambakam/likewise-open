AM_CPPFLAGS = \
	-I$(top_srcdir)/netlogon/include \
	-I$(top_srcdir)/lwreg/include \
	-I$(top_srcdir)/lwadvapi/include \
	-I$(top_srcdir)/lwadvapi/include/public \
	-I$(top_srcdir)/lwmsg/include \
	-I$(top_srcdir)/lwbase/include \
	-I$(top_srcdir)/include

noinst_LTLIBRARIES=liblwnetutils.la

liblwnetutils_la_SOURCES = \
    globals.c           \
	lwnet-cfg.c         \
	lwnet-dlinkedlist.c \
	lwnet-dns.c         \
	lwnet-futils.c      \
	lwnet-info.c        \
	lwnet-logger.c      \
	lwnet-mem.c         \
	lwnet-paths.c       \
	lwnet-stack.c       \
	lwnet-time.c        \
	lwnet-sysfuncs.c

noinst_HEADERS = \
    externs.h       \
    includes.h      \
	lwnet-dns_p.h

liblwnetutils_la_LIBADD =  \
	@top_builddir@/lwreg/shellutil/librsutils.la \
	@top_builddir@/lwreg/client/libregclient.la \
	@top_builddir@/lwreg/common/libregcommon.la \
	@RESOLV_LIBS@  \
	@RT_LIBS@      \
	@PTHREAD_LIBS@


