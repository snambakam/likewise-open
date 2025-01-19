noinst_LTLIBRARIES = libregutils.la

# General configuration

AM_CFLAGS = @LWMSG_CFLAGS@
AM_CPPFLAGS = \
	@LWMSG_CPPFLAGS@ \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwbase/include \
	-I$(top_builddir)/include

libregutils_la_SOURCES = \
    consolelog.c         \
    filelog.c            \
    fileutils.c          \
    globals.c            \
    reglogger.c          \
    regloginfo.c         \
    regmem.c             \
    sysfuncs.c           \
    syslog.c             \
    regtime.c            \
    dlinkedlist.c        \
    hashtable.c          \
    regerror.c           \
    regdatatype.c
    
noinst_HEADERS = \
    externs.h       \
    includes.h

libregutils_la_CPPFLAGS = $(AM_CPPFLAGS) @LWBASE_INCLUDES@
libregutils_la_LDFLAGS = @LWBASE_LDFLAGS@
libregutils_la_LIBADD = @LWBASE_LIBS@
