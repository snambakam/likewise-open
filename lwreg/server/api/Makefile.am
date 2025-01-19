noinst_LTLIBRARIES = libregserverapi.la

libregserverapi_la_SOURCES = \
    main.c                \
    globals.c             \
    ipc_registry.c        \
    regserver.c           \
    regsecurity.c                 

libregserverapi_la_CPPFLAGS =        \
	-DLW_ENABLE_THREADS=1         \
	$(AM_CPPFLAGS)                \
	-I@top_srcdir@/lwreg/server/include \
    -I@top_srcdir@/lwreg/include \
    -I@top_srcdir@/lwmsg/include \
    -I@top_srcdir@/lwbase/include \
	@REG_BACKEND_DEFINES@

libregserverapi_la_LDFLAGS = \
    -L@top_builddir@/lwbase/src

if USE_SQLITE_BACKEND

BACKEND_LIB = \
    @top_builddir@/lwreg/server/providers/sqlite/libregserver_provider_sqlite.la

endif

if USE_FILE_BACKEND

BACKEND_LIB = \
    @top_builddir@/lwreg/server/providers/file/libregserver_provider_file.la

endif

libregserverapi_la_LIBADD = \
    $(BACKEND_LIB) \
    -llwbase \
    -llwmsg_nothr \
    -luuid \
    @PTHREAD_LIBS@
