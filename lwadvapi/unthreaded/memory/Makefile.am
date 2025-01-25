AM_CPPFLAGS = \
       -I$(top_srcdir)/lwadvapi/include \
       -I$(top_srcdir)/lwadvapi/include/public \
       -I$(top_srcdir)/lwmsg/include \
       -I$(top_srcdir)/lwbase/include

noinst_LTLIBRARIES = liblwadvapi_memory.la

liblwadvapi_memory_la_SOURCES = \
       sysfuncs.c    \
       lwmem.c    \
       lwstr.c    \
       lwsid.c    \
       lwbuffer.c \
       lwhash.c

liblwadvapi_memory_la_CPPFLAGS = $(AM_CPPFLAGS)
liblwadvapi_memory_la_LIBADD =
liblwadvapi_memory_la_LDFLAGS =
