AM_CPPFLAGS = \
    -I$(top_srcdir)/lwadvapi/include \
    -I$(top_srcdir)/lwadvapi/include/public \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

noinst_LTLIBRARIES = liblwadvapi_lwunistr.la

liblwadvapi_lwunistr_la_SOURCES = lwunistr.c
liblwadvapi_lwunistr_la_LIBADD = \
    $(top_builddir)/lwbase/src/liblwbase_nothr.la
liblwadvapi_lwunistr_la_CPPFLAGS = $(AM_CPPFLAGS)
