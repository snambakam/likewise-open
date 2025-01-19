bin_PROGRAMS = lwregshell
bin_SCRIPTS = lw-edit-reg

#lwregshell
lwregshell_SOURCES = \
    import.c \
    export.c \
    cmdparse.c \
    regshell.c

lwregshell_LDADD = \
        @top_builddir@/lwreg/client/libregclient.la \
        @top_builddir@/lwreg/shellutil/librsutils.la \
        @top_builddir@/lwreg/parse/libregparse.la \
        -llwbase_nothr \
        -llwmsg \
        @LIBEDIT_LIBS@

lwregshell_CPPFLAGS = \
    $(AM_CPPFLAGS)   \
    -I@top_srcdir@/lwreg/shellutil \
    -I@top_srcdir@/lwreg/include \
    -I@top_srcdir@/lwmsg/include \
    -I@top_srcdir@/lwbase/include \
    -Wno-restrict \
    @LIBEDIT_INCLUDES@

lwregshell_LDFLAGS = \
    -L@top_builddir@/lwmsg/src \
    -L@top_builddir@/lwbase/src \
    @LIBEDIT_LDFLAGS@

lw-edit-reg: lw-edit-reg.sh
	cp $< $@
	chmod +x $@
