AC_PREREQ([2.71])
AC_INIT([likewise-open],[1.3.0])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_SILENT_RULES([yes])
m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR
LT_INIT
AC_PROG_CC
AC_PROG_CXX(g++)
LT_INIT
AC_PREFIX_DEFAULT(/usr)

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

dnl Check supported operating systems
dnl

case "${host_os}:${host_cpu}" in
    linux*:x86_64)
        PLATFORM_LIB_PREFIX=lib64
        ;;
    linux*:aarch64)
        PLATFORM_LIB_PREFIX=lib
        ;;
    darwin*:x86_64)
        PLATFORM_LIB_PREFIX=lib
        ;;
    *)
        AC_MSG_ERROR("Unsupported operating system - ${host_os}:${host_cpu}")
        ;;
esac

CPPFLAGS="$CPPFLAGS -D_REENTRANT -D_GNU_SOURCE -fPIC"
CXXFLAGS="$CXXFLAGS -frtti"

AM_CPPFLAGS="$AM_CPPFLAGS -I${top_srcdir}/include"
AM_CFLAGS="$AM_CFLAGS -Wall -Werror -fno-strict-aliasing"

#flag overrides to allow per module compile flags.
#warning ignore flags are here (temporarily) so that modules
#can start fixing code by overriding in respective Makefile.am
#to check a module, do AM_CFLAGS=$(CFLAGS_DESIRED) in Makefile.am
CFLAGS_ORIGINAL="$AM_CFLAGS"
CFLAGS_COMPAT="-Wno-unused-but-set-variable -Wno-pointer-sign -Wno-implicit-function-declaration -Wno-address -Wno-enum-compare"
CFLAGS_DESIRED="$CFLAGS_ORIGINAL -fno-strict-aliasing -Wformat -Wformat-security -Werror=format-security -fstack-protector-all"
#reset cflags to not break existing
#AM_CFLAGS="$AM_CFLAGS $CFLAGS_COMPAT"
AM_CFLAGS="$AM_CFLAGS $CFLAGS_DESIRED"

AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(CFLAGS_ORIGINAL)
AC_SUBST(CFLAGS_COMPAT)
AC_SUBST(CFLAGS_DESIRED)

AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_CHECK_HEADERS(pthread.h errno.h sys/types.h sys/stat.h stdio.h string.h strings.h)
AC_CHECK_HEADERS(unistd.h time.h inttypes.h sys/socket.h netdb.h syslog.h)
AC_CHECK_HEADERS(stdlib.h locale.h stddef.h stdarg.h assert.h signal.h)
AC_CHECK_HEADERS(ctype.h netinet/in.h arpa/inet.h fcntl.h sys/un.h dirent.h termios.h term.h)
AC_CHECK_HEADERS(pwd.h grp.h nss.h nss_common.h nsswitch.h shadow.h crypt.h)
AC_CHECK_HEADERS(limits.h)

AC_C_CONST
AC_TYPE_SIZE_T

AC_FUNC_VPRINTF
AC_CHECK_FUNCS(strerror)

#--------------------------------------------------------------------

AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],[enable debugging (default: disabled)])],
    [
        if test x"$enableval" = x"yes"
        then
            AM_CFLAGS="$AM_CFLAGS -g -O0"
            AM_CPPFLAGS="$AM_CPPFLAGS -DDEBUG -DLDAP_DEBUG"
        fi
    ])

#--------------------------------------------------------------------
# openssl component

AC_ARG_WITH([ssl],
    [AS_HELP_STRING([--with-ssl=<dir>],[use SSL binaries rooted at prefix <dir>])],
    [
        OPENSSL_BASE_PATH="$withval"
    ],
    [
        OPENSSL_BASE_PATH="/usr"
    ])

AC_ARG_WITH([ssl-includes],
    [AS_HELP_STRING([--with-ssl-includes=<dir>],[use SSL headers located in prefix <dir>])],
    [
        OPENSSL_INCLUDES="-DOPENSSL_NO_KRB5 -I$withval"
    ],
    [
        OPENSSL_INCLUDES="-DOPENSSL_NO_KRB5 -I$OPENSSL_BASE_PATH/include"
    ])

AC_ARG_WITH([ssl-libs],
    [AS_HELP_STRING([--with-ssl-libs=<dir>],[use SSL libraries located in prefix <dir>])],
    [
        OPENSSL_LDFLAGS="-L$withval"
    ],
    [
        OPENSSL_LDFLAGS="-L$OPENSSL_BASE_PATH/$PLATFORM_LIB_PREFIX -Wl,-rpath,$OPEN_SSL_DEFAULT_PATH"
    ])

AC_SUBST(OPENSSL_BASE_PATH)
AC_SUBST(OPENSSL_INCLUDES)
AC_SUBST(OPENSSL_LDFLAGS)

saved_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $LW_INCLUDES"
AC_CHECK_HEADERS(openssl/crypto.h openssl/ssl.h openssl/err.h)
CPPFLAGS="$saved_CPPFLAGS"

#--------------------------------------------------------------------
# zlib component

AC_ARG_WITH([zlib],
    [AS_HELP_STRING([--with-zlib=<dir>],[use zlib binaries rooted at prefix <dir>])],
    [
        ZLIB_BASE_PATH="$withval"
    ],
    [
        ZLIB_BASE_PATH="/usr"
    ])

ZLIB_INCLUDES="-I$ZLIB_BASE_PATH/include"
ZLIB_LDFLAGS="-L$ZLIB_BASE_PATH/lib -Wl,-rpath,/opt/vmware/lib64 -Wl,-rpath-link,/opt/vmware/lib64"

AC_SUBST(ZLIB_BASE_PATH)
AC_SUBST(ZLIB_INCLUDES)
AC_SUBST(ZLIB_LDFLAGS)

#--------------------------------------------------------------------
# bzip component

AC_ARG_WITH([bzip],
    [AS_HELP_STRING([--with-bzip=<dir>],[use BZIP binaries rooted at prefix <dir>])],
    [
        BZIP_BASE_PATH="$withval"
    ],
    [
        BZIP_BASE_PATH="/usr"
    ])

BZIP_INCLUDES="-I$BZIP_BASE_PATH/include"
BZIP_LDFLAGS="-L$BZIP_BASE_PATH/lib -Wl,-rpath,/usr/lib64 -Wl,-rpath-link,/usr/lib64"

AC_SUBST(BZIP_BASE_PATH)
AC_SUBST(BZIP_INCLUDES)
AC_SUBST(BZIP_LDFLAGS)

AC_CHECK_LIB([dl],
    [dlopen],
    [DL_LIBS="-ldl"])

AC_CHECK_LIB([pthread],
    [pthread_self],
    [PTHREAD_LIBS="-lpthread"])

AC_CHECK_LIB([z],
    [inflate],
    [ZLIB_LIBS="-lz"],
    [],
    [$ZLIB_LDFLAGS])

AC_CHECK_LIB([bz2],
    [BZ2_bzdopen],
    [BZIP_LIBS="-lbz2"],
    [],
    [$BZIP_LDFLAGS])

AC_CHECK_LIB([uuid],
    [uuid_copy],
    [UUID_LIBS="-luuid"],
    [],
    [$LW_LDFLAGS -luuid])

AC_CHECK_LIB([crypto],
    [MD5_Init],
    [CRYPTO_LIBS="-lcrypto -lssl"],
    [],
    [$OPENSSL_LDFLAGS])

AC_CHECK_LIB([lber],
    [ber_scanf],
    [LBER_LIBS="-llber"],
    [],
    [$LW_LDFLAGS -llber])

AC_CHECK_LIB([gssapi_krb5],
    [gss_accept_sec_context],
    [GSSAPI_LIBS="-lgssapi_krb5 -lkrb5 -lk5crypto -lkrb5support -lcom_err"],
    [],
    [$LW_LDFLAGS -lgssapi_krb5 -lkrb5 -lk5crypto -lkrb5support -lcom_err])

AC_CHECK_LIB([ldap_r],
    [ldap_initialize],
    [LDAP_LIBS="-lldap_r -llber"],
    [],
    [$LW_LDFLAGS -llber $SASL_LDFLAGS $SASL_LIBS $GSSAPI_LIBS $OPENSSL_LDFLAGS $CRYPTO_LIBS])

AC_CHECK_LIB([sqlite3],
    [sqlite3_open],
    [SQLITE_LIBS="-lsqlite3"],
    [],
    [$SQLITE_LDFLAGS $DL_LIBS $PTHREAD_LIBS])

#--------------------------------------------------------------------

AC_SUBST(DL_LIBS)
AC_SUBST(PTHREAD_LIBS)
AC_SUBST(ZLIB_LIBS)
AC_SUBST(BZIP_LIBS)

#--------------------------------------------------------------------

gitcommithash=`echo $(git rev-parse --short HEAD)`
AC_SUBST(gitcommithash)
AC_DEFINE_UNQUOTED(GIT_COMMIT_HASH, "$gitcommithash", [Git Commit Hash])

#--------------------------------------------------------------------

AC_CONFIG_FILES([Makefile
                ])

AC_OUTPUT
