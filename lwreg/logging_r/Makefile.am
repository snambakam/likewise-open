noinst_LTLIBRARIES = libreglog_r.la

libreglog_r_la_SOURCES = \
	globals.c \
	logging.c 

libreglog_r_la_CPPFLAGS = \
	$(AM_CPPFLAGS) \
    -DLW_ENABLE_THREADS=1 \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwbase/include

libreglog_r_la_LIBADD = \
	@top_builddir@/lwreg/utils/libregutils.la \
	@PTHREAD_LIBS@

noinst_HEADERS =   \
	includes.h \
	externs.h
