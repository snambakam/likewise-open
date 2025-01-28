noinst_LTLIBRARIES = liblwioutils.la

liblwioutils_la_SOURCES = \
    assert.c      \
    bitvector.c   \
    consolelog.c  \
    ecp.c         \
    dlinkedlist.c \
    filelog.c     \
    fileutils.c   \
    globals.c     \
    hashtable.c   \
    hostinfo.c    \
    libmain.c     \
    logger.c      \
    lwlist.c      \
    memory.c      \
    multistring.c \
    queue.c       \
    lwiologinfo.c \
    path.c        \
    stack.c       \
    strings.c     \
    sysfuncs.c    \
    syslog.c      \
    time.c        \
    unistr.c      \
    regcfg.c      \
    lru.c

liblwioutils_la_CPPFLAGS = \
    $(AM_CPPFLAGS) \
    -I@top_srcdir@/lwio/include \
    -I@top_srcdir@/lwreg/include \
    -I@top_srcdir@/lwbase/include

liblwioutils_la_LIBADD = \
    @top_builddir@/lwbase/src/liblwbase.la \
    @top_builddir@/lwreg/client/libregclient.la \
    @top_builddir@/lwreg/common/libregcommon.la \
    @DL_LIBS@     \
    @RT_LIBS@

liblwioutils_la_LDFLAGS = \
   $(AM_LDFLAGS) \
   @LWBASE_LDFLAGS@
