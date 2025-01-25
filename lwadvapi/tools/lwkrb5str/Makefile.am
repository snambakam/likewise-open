noinst_PROGRAMS = lwkrb5str

lwkrb5str_SOURCES = \
    main.c

lwkrb5str_CPPFLAGS =  \
    -I@top_srcdir@         \
    -I@top_srcdir@/lwadvapi/include \
    -I@top_srcdir@/lwadvapi/include/public \
    -I@top_srcdir@/lwmsg/include \
    -I@top_srcdir@/lwbase/include \
    $(KRB5_INCLUDES)

lwkrb5str_LDADD = \
    @top_builddir@/lwadvapi/threaded/liblwadvapi_threaded.la \
    @top_builddir@/lwadvapi/unthreaded/liblwadvapi_nothr.la \
    $(KRB5_LIBS)

lwkrb5str_LDFLAGS = \
    $(KRB5_LDFLAGS)

