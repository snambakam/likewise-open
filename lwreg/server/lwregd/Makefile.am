sbin_PROGRAMS = lwregd

lwregd_SOURCES =    \
    globals.c       \
    listener.c      \
    main.c          \
    sighandler.c

lwregd_CPPFLAGS =                 \
    -DLW_ENABLE_THREADS=1         \
    $(AM_CPPFLAGS)                \
    -I@top_srcdir@/lwreg/server/include \
    -I@top_srcdir@/lwreg/include \
    -I@top_srcdir@/lwmsg/include \
    -I@top_srcdir@/lwbase/include

lwregd_LDADD =                             \
    @top_builddir@/lwreg/server/api/libregserverapi.la \
    @top_builddir@/lwreg/common/libregcommon.la     \
    @top_builddir@/lwreg/logging_r/libreglog_r.la   \
    @DS_FRAMEWORK_LDFLAGS@ \
    @top_builddir@/lwmsg/src/liblwmsg.la

lwregd_LDFLAGS = \
    -L@top_builddir@/lwmsg/src \
    -L@top_builddir@/lwbase/src

lwregreg_DATA = lwreg.reg

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(registrycachedir)
