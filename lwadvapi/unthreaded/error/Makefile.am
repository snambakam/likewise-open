noinst_LTLIBRARIES = liblwadvapi_error.la

liblwadvapi_error_la_SOURCES = \
    error.c                    \
    error-conv.c

liblwadvapi_error_la_CPPFLAGS = \
    -I$(top_srcdir)/lwadvapi/include \
    -I$(top_srcdir)/lwadvapi/include/public \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

liblwadvapi_error_la_LIBADD = \
    $(top_builddir)/lwbase/src/liblwbase_nothr.la
