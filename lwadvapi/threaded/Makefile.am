AM_CPPFLAGS = \
	-I$(top_srcdir)/lwadvapi/include \
	-I$(top_srcdir)/lwadvapi/include/public \
	-I$(top_srcdir)/lwmsg/include \
	-I$(top_srcdir)/lwbase/include \
	$(CYRUSSASL_INCLUDES) \
	$(OPENLDAP_INCLUDES) \
	$(KRB5_INCLUDES)

noinst_LTLIBRARIES = liblwadvapi_threaded.la

liblwadvapi_threaded_la_SOURCES = \
       globals.c  \
       krbtgt.c \
       lwkrb5.c \
       lwldap.c   \
       lwsecurityidentifier.c   \
       lwtime.c

liblwadvapi_threaded_la_CPPFLAGS = \
	$(AM_CPPFLAGS) \
    -DLW_ENABLE_THREADS=1

liblwadvapi_threaded_la_LIBADD = \
	$(top_builddir)/lwbase/src/liblwbase.la \
	$(OPENLDAP_LIBS) \
	$(RT_LIBS) \
	$(KRB5_LIBS)

liblwadvapi_threaded_la_LDFLAGS = \
	$(OPENLDAP_LDFLAGS) \
	$(KRB5_LDFLAGS)
