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

MOD_EXT=".so"
REALLY_X64=""
DS_FRAMEWORK_LDFLAGS=""

case "$host_os:$host_cpu" in
        solaris*:*)
                if echo "$CC" | grep -- "-m64" >/dev/null 2>&1
                then
                        REALLY_X64="yes"
                fi
                LWBASE_CPPFLAGS="$LWBASE_CPPFLAGS -D_XOPEN_SOURCE=500 -D_EXTENSIONS -D_REENTRANT"
                ;;
        hpux*:hppa*)
                LWBASE_CPPFLAGS="$LWBASE_CPPFLAGS -D_XOPEN_SOURCE_EXTENDED=1 -D_XOPEN_SOURCE=500"
                MOD_EXT=".sl"
                AC_DEFINE([__LWI_HP_UX__], [], [Define if OS is HP-UX])
                ;;
        hpux*:ia64*)
                LWBASE_CPPFLAGS="$LWBASE_CPPFLAGS -D_XOPEN_SOURCE_EXTENDED=1 -D_XOPEN_SOURCE=500"
                AC_DEFINE([__LWI_HP_UX__], [], [Define if OS is HP-UX])
                ;;
        aix*:*)
                LWBASE_CPPFLAGS="$LWBASE_CPPFLAGS -D_THREAD_SAFE"
                ;;
        darwin*:*)
                if test "`sysctl -n hw.optional.x86_64 2>/dev/null`" = "1"
                then
                        REALLY_X64="yes"
                fi
                DS_FRAMEWORK_LDFLAGS="-framework DirectoryService"
                AC_DEFINE([__LWI_DARWIN__], [], [Define if OS is Darwin])
                ;;
esac

case "$host_cpu" in
        i?86)
                if test -n "$REALLY_X64" && echo "$CC" | grep -v -- "-arch i386" >/dev/null 2>&1
                then
                        AC_DEFINE([LWBASE_ATOMIC_X86_64], [], [x86_64 atomic operations])
                else
                        AC_DEFINE([LWBASE_ATOMIC_X86], [], [x86 atomic operations])
                fi
                ;;
        x86_64)
                # Check for -m32 in CC
                if echo "$CC" | grep -- "-m32" >/dev/null 2>&1
                then
                        AC_DEFINE([LWBASE_ATOMIC_X86], [], [x86 atomic operations])
                else
                        AC_DEFINE([LWBASE_ATOMIC_X86_64], [], [x86_64 atomic operations])
                fi
                ;;
        *)
                AC_DEFINE([LWBASE_ATOMIC_LOCK], [], [lock atomic operations])
                ;;
esac

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

AS_AC_EXPAND(LIBDIR, $libdir)
AC_DEFINE_UNQUOTED(LIBDIR, "$LIBDIR", [Library directory])
AC_DEFINE_UNQUOTED([MOD_EXT], ["$MOD_EXT"], [Extension of shared modules])

AC_SUBST(MOD_EXT)
AC_SUBST(LWBASE_CPPFLAGS)
AC_SUBST(LWBASE_CFLAGS)
AC_SUBST(LWBASE_LDFLAGS)

AC_SUBST(DS_FRAMEWORK_LDFLAGS)

CPPFLAGS="$CPPFLAGS -D_REENTRANT -D_GNU_SOURCE -fPIC"
CXXFLAGS="$CXXFLAGS -frtti"

AM_CPPFLAGS="$AM_CPPFLAGS -I$srcdir/include"
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
AC_CHECK_HEADERS(synch.h sys/param.h limits.h)

AC_C_CONST
AC_TYPE_SIZE_T

AC_FUNC_VPRINTF
AC_CHECK_FUNCS(strerror)
AC_CHECK_FUNCS([strndup strncasecmp strtoul strtoll __strtoll])
AC_CHECK_FUNCS([strtoll __strtoll strtoull __strtoull strtol strtoul])
AC_CHECK_FUNCS([strtoll __strtoll strtoull __strtoull strtol strtoul])
AC_CHECK_FUNCS([strerror mktime strdup memmove mktime timegm strlcpy])
AC_CHECK_FUNCS([strlcat strndup strnlen setenv unsetenv seteuid setegid])
AC_CHECK_FUNCS([setlinebuf strcasestr strtok_r strtoll strtoull ftruncate])
AC_CHECK_FUNCS([initgroups bzero memset dlerror dlopen dlsym dlclose])
AC_CHECK_FUNCS([socketpair vasprintf snprintf vsnprintf asprintf])
AC_CHECK_FUNCS([vsyslog va_copy __va_copy strptime mkdtemp])
AC_CHECK_FUNCS([strtoll __strtoll strtoull __strtoull strtol strtoul])
AC_CHECK_FUNCS([strndup strncasecmp strtoul strtoll __strtoll])

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

#--------------------------------------------------------------------
AC_CHECK_HEADERS([wctype.h])

AC_CHECK_LIB(iconv, iconv, [LIB_ICONV="-liconv"],
AC_CHECK_LIB(iconv, libiconv, [LIB_ICONV="-liconv"], [LIB_ICONV=""]))

AC_SUBST(LIB_ICONV)

saved_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Wall -Werror"

AC_MSG_CHECKING([whether iconv() takes const char** as its second parameter])

AC_TRY_COMPILE(
[#include <iconv.h>
 #include <stddef.h>
 ],
[iconv_t h = iconv_open(NULL, NULL);
    char* in;
    (void) iconv(h, (const char**) &in, NULL, NULL, NULL);
],
[AC_MSG_RESULT([yes]); AC_DEFINE_UNQUOTED(ICONV_IN_TYPE, [const char**], [iconv in type])],
[AC_MSG_RESULT([no]); AC_DEFINE_UNQUOTED(ICONV_IN_TYPE, [char**], [iconv in type])])

CFLAGS="$saved_CFLAGS"

AC_CHECK_FUNCS([wcscasecmp])

#--------------------------------------------------------------------

# Check for epoll

AC_CHECK_HEADERS([sys/epoll.h], [HAVE_EPOLL_H=true], [HAVE_EPOLL_H=false])
AC_CHECK_FUNC([epoll_create], [HAVE_EPOLL_CREATE=true], [HAVE_EPOLL_CREATE=false])

# Check for kqueue

AC_CHECK_FUNC([kqueue], [HAVE_KQUEUE=true], [HAVE_KQUEUE=false])

#--------------------------------------------------------------------

# Check for CPU affinity functions
saved_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS $LIB_PTHREAD"
AC_CHECK_FUNCS([pthread_setaffinity_np pthread_attr_setaffinity_np])
LDFLAGS="$saved_LDFLAGS"

#--------------------------------------------------------------------

# Check for moonunit bits
AC_CHECK_HEADERS([moonunit/moonunit.h], [HAVE_MOONUNIT_H=true], [HAVE_MOONUNIT_H=false])
AC_CHECK_LIB(moonunit, Mu_Interface_Result, [HAVE_MOONUNIT_LIB=true], [HAVE_MOONUNIT_LIB=false])
AC_CHECK_PROG(MOONUNIT_STUB, moonunit-stub, moonunit-stub, false)

if test "${MOONUNIT_STUB}" = "moonunit-stub" && $HAVE_MOONUNIT_H && $HAVE_MOONUNIT_LIB
then
        HAVE_MOONUNIT="true"
        LIB_MOONUNIT="-lmoonunit"
        AC_DEFINE([HAVE_MOONUNIT], [], [Have MoonUnit])
else
        HAVE_MOONUNIT="false"
fi

AC_SUBST(LIB_MOONUNIT)
AM_CONDITIONAL(HAVE_MOONUNIT, $HAVE_MOONUNIT)

#--------------------------------------------------------------------

HAVE_DOXYGEN=false
AC_ARG_ENABLE([doxygen],
    [AS_HELP_STRING([--enable-doxygen],[enable doxygen for generating documentation (default: disabled)])],
    [
        if test x"$enableval" = x"yes"
        then
            HAVE_DOXYGEN=true
            AC_DEFINE(HAVE_DOXYGEN, 1, [use doxygen to generate documentation])
        fi
    ]
)
AM_CONDITIONAL(HAVE_DOXYGEN, [$HAVE_DOXYGEN])

HAVE_DOXYGEN_PDF=false
AC_ARG_ENABLE([doxygen-pdf],
    [AS_HELP_STRING([--enable-doxygen-pdf],[enable doxygen pdf for generating documentation in pdf form (default: disabled)])],
    [
        if test x"$enableval" = x"yes"
        then
            HAVE_DOXYGEN_PDF=true
            AC_DEFINE(HAVE_DOXYGEN_PDF, 1, [use doxygen pdf to generate documentation])
        fi
    ]
)
AM_CONDITIONAL(HAVE_DOXYGEN_PDF, [$HAVE_DOXYGEN_PDF])

#--------------------------------------------------------------------

# libuuid

AC_ARG_WITH([libuuid],
        [AC_HELP_STRING([--with-libuuid=<dir>], [use libuuid located in prefix <dir>])],
        [
                LIBUUID_INCLUDES="-I$withval/include"
                LIBUUID_LDFLAGS="-L$withval/lib"
        ])

AC_ARG_WITH([libuuid-includes],
        [AC_HELP_STRING([--with-libuuid-includes=<dir>], [use libuuid includes located in <dir>])],
        [
                LIBUUID_INCLUDES="-I$withval/include"
        ])

AC_ARG_WITH([libuuid-libs],
        [AC_HELP_STRING([--with-libuuid-libs=<dir>], [use libuuid libs located in <dir>])],
        [
                LIBUUID_LDFLAGS="-L$withval/lib"
        ])

LIBUUID_LIBS="-luuid"

AC_SUBST(LIBUUID_INCLUDES)
AC_SUBST(LIBUUID_LDFLAGS)
AC_SUBST(LIBUUID_LIBS)

AC_CHECK_LIB([uuid], [uuid_generate_time], [LIBUUID_LIBS="-luuid"], [], [$LIBUUID_LDFLAGS])

#--------------------------------------------------------------------

# sqlite

AC_ARG_WITH([sqlite],
        [AC_HELP_STRING([--with-sqlite=<dir>], [use sqlite located in prefix <dir>])],
        [
        SQLITE_INCLUDES="-I$withval/include"
                SQLITE_LDFLAGS="-L$withval/lib"
        ])

AC_ARG_WITH([sqlite-includes],
        [AC_HELP_STRING([--with-sqlite-includes=<dir>], [use sqlite includes located in <dir>])],
        [
        SQLITE_INCLUDES="-I$withval"
        ])

AC_ARG_WITH([sqlite-libs],
        [AC_HELP_STRING([--with-sqlite-libs=<dir>], [use sqlite libs located in <dir>])],
        [
        SQLITE_LDFLAGS="-L$withval"
        ])

SQLITE_LIBS="-lsqlite3"

AC_SUBST(SQLITE_INCLUDES)
AC_SUBST(SQLITE_LDFLAGS)
AC_SUBST(SQLITE_LIBS)

#--------------------------------------------------------------------

# libedit

AC_ARG_WITH([libedit],
        [AC_HELP_STRING([--with-libedit=<dir>], [use libedit located in prefix <dir>])],
        [
        LIBEDIT_INCLUDES="-I$withval/include"
        LIBEDIT_LDFLAGS="-L$withval/lib"
        ])

AC_ARG_WITH([libedit-includes],
        [AC_HELP_STRING([--with-libedit-includes=<dir>], [use libedit includes located in <dir>])],
        [
        LIBEDIT_INCLUDES="-I$withval"
        ])

AC_ARG_WITH([libedit-libs],
        [AC_HELP_STRING([--with-libedit-libs=<dir>], [use libedit libs located in <dir>])],
        [
        LIBEDIT_LDFLAGS="-L$withval"
        ])

LIBEDIT_LIBS="-ledit"

AC_SUBST(LIBEDIT_INCLUDES)
AC_SUBST(LIBEDIT_LDFLAGS)
AC_SUBST(LIBEDIT_LIBS)

#--------------------------------------------------------------------

THREADPOOL_BACKEND=poll

if $HAVE_EPOLL_H && $HAVE_EPOLL_CREATE
then
        THREADPOOL_BACKEND=epoll
fi

if $HAVE_KQUEUE
then
        THREADPOOL_BACKEND=kqueue
fi

if test "$THREADPOOL_BACKEND" = "select"
then
        USE_SELECT=true
        AC_DEFINE([USE_SELECT], [], [Define to use select backend])
else
        USE_SELECT=false
fi

if test "$THREADPOOL_BACKEND" = "poll"
then
        USE_POLL=true
        AC_DEFINE([USE_POLL], [], [Define to use poll backend])
else
        USE_POLL=false
fi

if test "$THREADPOOL_BACKEND" = "kqueue"
then
        USE_KQUEUE=true
        AC_DEFINE([USE_KQUEUE], [], [Define to use kqueue backend])
else
        USE_KQUEUE=false
fi

if test "$THREADPOOL_BACKEND" = "epoll"
then
        USE_EPOLL=true
        AC_DEFINE([USE_EPOLL], [], [Define to use epoll backend])
else
        USE_EPOLL=false
fi

AM_CONDITIONAL(USE_SELECT, $USE_SELECT)
AM_CONDITIONAL(USE_POLL, $USE_POLL)
AM_CONDITIONAL(USE_EPOLL, $USE_EPOLL)
AM_CONDITIONAL(USE_KQUEUE, $USE_KQUEUE)

LWBASE_INCLUDES="-I$srcdir/lwbase/include"

AC_SUBST(LWBASE_INCLUDES)

#--------------------------------------------------------------------

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
if test "x$ac_cv_c_bigendian" = "xyes"; then
    AC_DEFINE([LW_BIG_ENDIAN], [], [Define standard macro for Big Endian systems])
else
    AC_DEFINE([LW_LITTLE_ENDIAN], [], [Define standard macro for Little Endian systems])
fi

AC_CHECK_SIZEOF(wchar_t)

#--------------------------------------------------------------------

# Check for cpu set type
AC_CHECK_TYPE(
        [cpu_set_t],
        [AC_DEFINE_UNQUOTED(CPU_SET_TYPE, [cpu_set_t], [CPU set type])],
        [],
        [
#ifdef HAVE_PTHREAD_H
#include <pthread.h>    
#endif
])
AC_CHECK_TYPE(
        [cpuset_t],
        [AC_DEFINE_UNQUOTED(CPU_SET_TYPE, [cpuset_t], [CPU set type])],
        [],             
        [       
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_CPUSET_H
#include <sys/cpuset.h>
#endif
])

#--------------------------------------------------------------------

LWMSG_CPPFLAGS="-DLWMSG_DISABLE_DEPRECATED"
LWMSG_CFLAGS="-Wall -Werror -Wmissing-prototypes"
LWMSG_LDFLAGS=""

case "$host_os:$host_cpu" in
    linux*:*)
        LWMSG_CPPFLAGS="$LWMSG_CPPFLAGS -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE"
        ;;
esac

AC_CHECK_LIB(xnet, __xnet_connect, [LIB_XNET="-lxnet"], [LIB_XNET=""])

AC_SUBST(LWMSG_CPPFLAGS)
AC_SUBST(LWMSG_CFLAGS)
AC_SUBST(LWMSG_LDFLAGS)
AC_SUBST(LIB_XNET)

#--------------------------------------------------------------------

# backend

REG_BACKEND="sqlite"
USE_SQLITE_BACKEND=false
USE_FILE_BACKEND=false
REG_BACKEND_DEFINES=-DREG_USE_SQLITE

AC_ARG_WITH([backend],
        [AC_HELP_STRING([--with-backend=<backend>], [use backend located in prefix <backend>])],
        [
        REG_BACKEND = "$withval"      
        ])
        
if test x"$REG_BACKEND" = x"sqlite"
then
USE_SQLITE_BACKEND=true;
REG_BACKEND_DEFINES=-DREG_USE_SQLITE
fi

AM_CONDITIONAL(USE_SQLITE_BACKEND, [$USE_SQLITE_BACKEND])

if test x"$REG_BACKEND" = x"file"
then
USE_FILE_BACKEND=true;
REG_BACKEND_DEFINES=-DREG_USE_FILE
fi

AM_CONDITIONAL(USE_FILE_BACKEND, [$USE_FILE_BACKEND])

AC_SUBST(REG_BACKEND)
AC_SUBST(REG_BACKEND_DEFINES)

lwregregdir="$datadir/config"
AC_SUBST(lwregregdir)

if test x"$localstatedir" = x"/var"; then
    registrycachedir="$localstatedir/lib/likewise"
else
    registrycachedir="$localstatedir"
fi
AC_SUBST(registrycachedir)
AS_AC_EXPAND(CACHEDIR, $registrycachedir)
AC_DEFINE_UNQUOTED(CACHEDIR, "$CACHEDIR", [Cache directory])

AC_CHECK_DECLS([isblank], [], [], [#include <ctype.h>])

saved_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I$srcdir/lwbase/include"
AC_CHECK_HEADERS([wc16str.h wc16printf.h])
CPPFLAGS="$saved_CPPFLAGS"

AC_CHECK_TYPES([wchar16_t], [], [], [AC_INCLUDES_DEFAULT
#ifdef HAVE_WC16STR_H
# include <wc16str.h>
#endif
])

#--------------------------------------------------------------------

AC_CHECK_LIB([dl],
    [dlopen],
    [LIBDL="-ldl"])

AC_CHECK_LIB([pthread],
    [pthread_self],
    [LIB_PTHREAD="-lpthread"])

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

AC_CHECK_LIB([rt], [sigwaitinfo], [LIB_RT="-lrt"], [LIB_RT=""])

#--------------------------------------------------------------------

AC_SUBST(LIB_DL)
AC_SUBST(LIB_PTHREAD)
AC_SUBST(ZLIB_LIBS)
AC_SUBST(BZIP_LIBS)
AC_SUBST(LIB_RT)

#--------------------------------------------------------------------

gitcommithash=`echo $(git rev-parse --short HEAD)`
AC_SUBST(gitcommithash)
AC_DEFINE_UNQUOTED(GIT_COMMIT_HASH, "$gitcommithash", [Git Commit Hash])

#--------------------------------------------------------------------

AS_AC_EXPAND(PREFIXDIR, $prefix)
AC_DEFINE_UNQUOTED(PREFIXDIR, "$PREFIXDIR", [Prefix directory])

#--------------------------------------------------------------------

AC_CONFIG_FILES([
	Makefile
	lwbase/Makefile
	lwbase/doc/Makefile
	lwbase/include/Makefile
	lwbase/include/lw/Makefile
	lwbase/moonunit/Makefile
	lwbase/src/Makefile
	lwmsg/Makefile
    lwreg/Makefile
    lwreg/server/Makefile
    lwreg/server/providers/Makefile
    lwreg/server/providers/sqlite/Makefile
    lwreg/server/providers/file/Makefile
    lwreg/server/lwregd/lwreg.reg
    lwreg/server/lwregd/Makefile
    lwreg/server/api/Makefile
    lwreg/shellutil/Makefile
    lwreg/include/Makefile
    lwreg/utils/Makefile
    lwreg/logging_r/Makefile
    lwreg/ipc/Makefile
    lwreg/client/Makefile
    lwreg/shell/Makefile
    lwreg/shell/lw-edit-reg.sh
    lwreg/parse/Makefile
    lwreg/common/Makefile
])

AC_OUTPUT
