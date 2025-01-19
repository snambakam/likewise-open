noinst_LTLIBRARIES = libregipc.la

libregipc_la_SOURCES =    \
    ipc_protocol.c \
    ipcio.c
       
libregipc_la_CPPFLAGS = \
    $(AM_CPPFLAGS) \
    -I$(top_srcdir)/lwreg/include \
    -I$(top_srcdir)/lwmsg/include \
    -I$(top_srcdir)/lwbase/include
       
libregipc_la_LIBADD = \
    @LWMSG_LIBS@

