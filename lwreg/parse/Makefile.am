noinst_LTLIBRARIES = libregparse.la

libregparse_la_SOURCES = \
    regio.c \
    reglex.c \
    regiconv.c \
    regparse.c

libregparse_la_CPPFLAGS = \
    $(AM_CPPFLAGS)        \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include

libregparse_la_LIBADD = \
    @top_builddir@/lwreg/common/libregcommon.la

libregparse_la_LDFLAGS = @LWBASE_LDFLAGS@
