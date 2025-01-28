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

# compat

ENABLE_COMPAT=false

AC_ARG_ENABLE([compat],
        [AC_HELP_STRING([--enable-compat], [enable compat-only build (default: disabled)])],
        [
                if test x"$enableval" = x"yes"
                then
                        ENABLE_COMPAT=true
                else
                        ENABLE_COMPAT=false
                fi
        ])

AM_CONDITIONAL(ENABLE_COMPAT, [$ENABLE_COMPAT])

#--------------------------------------------------------------------

# netlogon

AC_ARG_ENABLE([minimal], [AC_HELP_STRING([--enable-minimal], [build minimal netlogon])])
AM_CONDITIONAL(ENABLE_MINIMAL, test x"$enable_minimal" = x"yes")
if test x"$enable_minimal" = x"yes"
then
        AC_DEFINE([ENABLE_MINIMAL], [], [Define if should build minimal netlogon])
fi

AS_AC_EXPAND(netlogonconfdir, ["$sysconfdir"])
AS_AC_EXPAND(netlogonregdir, ["$datadir/config"])

AC_DEFINE_UNQUOTED([LWNET_CONFIG_DIR], ["$netlogonconfdir"], [Location of netlogon configuration files])

if test x"$localstatedir" = x"/var"; then
    netlogoncachedir="$localstatedir/lib/likewise"
else
    netlogoncachedir="$localstatedir"
fi
AC_DEFINE_UNQUOTED([LWNET_CACHE_DIR], ["$netlogoncachedir"], [Location of netlogon cache files])

AC_CHECK_LIB([nsl], [gethostname], [NSL_LIBS="-lnsl"],,)
AC_CHECK_LIB([resolv], [res_query], [RESOLV_LIBS="-lresolv"],,)
AC_CHECK_LIB([resolv], [__res_query], [RESOLV_LIBS="-lresolv"],,)
AC_CHECK_LIB([rt], [clock_settime], [RT_LIBS="-lrt"],,)
AC_CHECK_LIB([socket], [bind], [SOCKET_LIBS="-lsocket"],,)

AC_SUBST(NSL_LIBS)
AC_SUBST(RESOLV_LIBS)
AC_SUBST(RT_LIBS)
AC_SUBST(SOCKET_LIBS)

old_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS $RT_LIBS"
AC_CHECK_FUNCS([clock_gettime clock_settime settimeofday gettimeofday timegm])
LDFLAGS="$old_LDFLAGS"

AC_CHECK_TYPES([socklen_t], [], [], [AC_INCLUDES_DEFAULT
#include <sys/types.h>
#include <sys/socket.h>
])

saved_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Wall -Werror"

AC_MSG_CHECKING([if getsockname takes socklen_t*])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],
[
int fd = -1;
struct sockaddr addr;
socklen_t addrlen;
(void) getsockname(fd, &addr, &addrlen);
], [
AC_MSG_RESULT([yes])
AC_DEFINE([GETSOCKNAME_TAKES_SOCKLEN_T], [], [Define if getsockname takes socklen_t as its third argument])],[
AC_MSG_RESULT([no])
])

AC_MSG_CHECKING([if struct msghdr has msg_control])
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
],
[
struct msghdr hdr;
hdr.msg_control = 0;
],
[
AC_MSG_RESULT([yes])
AC_DEFINE([MSGHDR_HAS_MSG_CONTROL], [], [Define if struct msghdr has the msg_control field])
],
[
AC_MSG_RESULT([no])
])

CFLAGS="$saved_CFLAGS"

#--------------------------------------------------------------------

AC_MSG_CHECKING([whether clock_settime crashes])
old_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS $RT_LIBS"
AC_RUN_IFELSE([AC_LANG_PROGRAM([[#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>]],
[[struct timespec timevar;
if (clock_gettime(CLOCK_REALTIME, &timevar) < 0)
{
        printf("clock_gettime failed with errno %d\n", errno);
        return 1;
}
if (clock_settime(CLOCK_PROCESS_CPUTIME_ID, &timevar) < 0)
{
        printf("clock_settime failed with errno %d\n", errno);
        return 0;
}]])],
        [AC_MSG_RESULT(no)],
        [AC_MSG_RESULT([yes])
        AC_DEFINE([CLOCK_SETTIME_CRASHES], [1], [If set, do not use clock_settime, because it crashes])])
LDFLAGS="$old_LDFLAGS"

#--------------------------------------------------------------------

KRB5_LIBS="-lgssapi_krb5 -lkrb5 -lk5crypto"
AC_SUBST(KRB5_LIBS)

#--------------------------------------------------------------------

OPENLDAP_LIBS="-lldap_r -llber"
AC_SUBST(OPENLDAP_LIBS)

#--------------------------------------------------------------------

CRYPTO_LIBS="-lcrypto"
AC_SUBST(CRYPTO_LIBS)

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
AC_CHECK_LIB([pthread], [pthread_self], [PTHREAD_LIBS="-lpthread"])

#--------------------------------------------------------------------

AC_SUBST(LIB_DL)
AC_SUBST(LIB_PTHREAD)
AC_SUBST(ZLIB_LIBS)
AC_SUBST(BZIP_LIBS)
AC_SUBST(LIB_RT)
AC_SUBST(UUID_LIBS)
AC_SUBST(PTHREAD_LIBS)

#--------------------------------------------------------------------

# share repository
share_repository=registry

AC_ARG_WITH([share-repository],
        [AC_HELP_STRING([--with-share-repository=<type>], [use share repository <type>])],
        [
                share_repository="$withval"
        ])

case "$share_repository" in

    sqlite)

        SHARE_REPOSITORY_TYPE_DEFINES=-DLW_USE_SHARE_REPOSITORY_SQLITE

        ;;

    registry)

        SHARE_REPOSITORY_TYPE_DEFINES=-DLW_USE_SHARE_REPOSITORY_REGISTRY

        ;;

    *)

        share_repository="registry"
        SHARE_REPOSITORY_TYPE_DEFINES=-DLW_USE_SHARE_REPOSITORY_REGISTRY

        ;;
esac

AM_CONDITIONAL(ENABLE_SHARE_REPOSITORY_SQLITE, test x"$share_repository" = x"sqlite")
AM_CONDITIONAL(ENABLE_SHARE_REPOSITORY_REGISTRY, test x"$share_repository" = x"registry")

AC_SUBST(SHARE_REPOSITORY_TYPE_DEFINES)

AC_ARG_WITH([drivers], [AC_HELP_STRING([--with-drivers], [list of drivers to build (npfs pvfs dfs srv rdr)])],
		       [], [with_drivers="npfs pvfs srv rdr"])

_IFS="${IFS}"
IFS=" ,"
for driver in $with_drivers
do
	eval "enable_${driver}=yes"
done
IFS="${_IFS}"

AM_CONDITIONAL(ENABLE_RDR, test x"$enable_rdr" = x"yes")
if test x"$enable_rdr" = x"yes"
then
	AC_DEFINE([ENABLE_RDR], [], [Define if rdr driver should be built])
fi

AM_CONDITIONAL(ENABLE_SRV, test x"$enable_srv" = x"yes")
if test x"$enable_srv" = x"yes"
then
	AC_DEFINE([ENABLE_SRV], [], [Define if srv driver should be built])
fi

AM_CONDITIONAL(ENABLE_PVFS, test x"$enable_pvfs" = x"yes")
if test x"$enable_pvfs" = x"yes"
then
	AC_DEFINE([ENABLE_PVFS], [], [Define if pvfs driver should be built])
fi

AM_CONDITIONAL(ENABLE_DFS, test x"$enable_dfs" = x"yes")
if test x"$enable_dfs" = x"yes"
then
	AC_DEFINE([ENABLE_DFS], [], [Define if dfs driver should be built])
fi

AM_CONDITIONAL(ENABLE_NPFS, test x"$enable_npfs" = x"yes")
if test x"$enable_npfs" = x"yes"
then
	AC_DEFINE([ENABLE_NPFS], [], [Define if npfs driver should be built])
fi

# Boolean settings

AC_ARG_ENABLE([static-drivers], [AC_HELP_STRING([--enable-static-drivers], [build drivers directly into lwiod])])
AM_CONDITIONAL(ENABLE_STATIC_DRIVERS, test x"$enable_static_drivers" = x"yes")
if test x"$enable_static_drivers" = x"yes"
then
	AC_DEFINE([ENABLE_STATIC_DRIVERS], [], [Define if drivers should be built directly into lwiod])
fi

AC_ARG_ENABLE([small-daemon], [AC_HELP_STRING([--enable-small-daemon], [build small lwiod])])
AM_CONDITIONAL(ENABLE_SMALL_DAEMON, test x"$enable_small_daemon" = x"yes")
if test x"$enable_small_daemon" = x"yes"
then
	AC_DEFINE([ENABLE_SMALL_DAEMON], [], [Define if lwiod should be small])
fi

ENABLE_IOTEST=true

AC_ARG_ENABLE([iotest],
        [AC_HELP_STRING([--disable-iotest], [disable iotest (default: enabled)])],
        [
                if test x"$enableval" = x"yes"
                then
                        ENABLE_IOTEST=true
                else
                        ENABLE_IOTEST=false
                fi
        ])

AM_CONDITIONAL(ENABLE_IOTEST, [$ENABLE_IOTEST])

saved_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -D_FILE_OFFSET_BITS=64"
AC_CHECK_HEADER([fuse.h], [HAVE_FUSE_H=true], [HAVE_FUSE_H=false])
CPPFLAGS="$saved_CPPFLAGS"

AC_CHECK_LIB(fuse, fuse_new, [HAVE_FUSE_LIB=true], [HAVE_FUSE_LIB=false])

if test x"$HAVE_FUSE_H" = x"true" -a x"$HAVE_FUSE_LIB" = x"true"
then
	HAVE_FUSE=true
else
	HAVE_FUSE=false
fi

AM_CONDITIONAL(ENABLE_FUSE, test x"$HAVE_FUSE" = x"true")

lwioconfdir="$sysconfdir"
AC_SUBST(lwioconfdir)
AS_AC_EXPAND(CONFIGDIR, $lwioconfdir)
AC_DEFINE_UNQUOTED(CONFIGDIR, "$CONFIGDIR", [Config directory])

lwioregdir="$datadir/config"
AC_SUBST(lwioregdir)

if test x"$localstatedir" = x"/var"; then
    lwiocachedir="$localstatedir/lib/likewise" 
else
    lwiocachedir="$localstatedir"
fi
AC_SUBST(lwiocachedir)
AS_AC_EXPAND(CACHEDIR, $lwiocachedir)
AC_DEFINE_UNQUOTED(CACHEDIR, "$CACHEDIR", [Cache directory])
        
AS_AC_EXPAND(providerdir, $libdir)

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
    lwadvapi/Makefile
    lwadvapi/include/Makefile
    lwadvapi/include/public/Makefile
    lwadvapi/unthreaded/Makefile
    lwadvapi/unthreaded/memory/Makefile
    lwadvapi/unthreaded/file/Makefile
    lwadvapi/unthreaded/dscache/Makefile
    lwadvapi/unthreaded/time/Makefile
    lwadvapi/unthreaded/lwunistr/Makefile
    lwadvapi/unthreaded/error/Makefile
    lwadvapi/threaded/Makefile
    lwadvapi/tools/Makefile
    lwadvapi/tools/lwkrb5str/Makefile
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
    netlogon/Makefile
    netlogon/etc/Makefile
    netlogon/etc/netlogond.reg
    netlogon/etc/netlogond.reg.svcm
    netlogon/include/Makefile
    netlogon/common/Makefile
    netlogon/ipc/Makefile
    netlogon/server/Makefile
    netlogon/server/ipc/Makefile
    netlogon/server/api/Makefile
    netlogon/server/netlogond/Makefile
    netlogon/client/Makefile
    netlogon/utils/Makefile
    netlogon/service_locator/Makefile
    netlogon/tools/get_log_info/Makefile
    netlogon/tools/export_dc_cache/Makefile
    netlogon/tools/Makefile
    netlogon/tools/get_dc_list/Makefile
    netlogon/tools/set_log_level/Makefile
    netlogon/tools/get_dc_info/Makefile
    netlogon/tools/get_dc_time/Makefile
    netlogon/tests/Makefile
    netlogon/tests/netbios/Makefile
    netlogon/tests/plugin/Makefile
    netlogon/tests/resolvehostclient/Makefile
    netlogon/tests/get_domain_controller/Makefile
    netlogon/tests/netlogonclient/Makefile
    lwio/Makefile
    lwio/etc/Makefile
    lwio/etc/dfs.reg
    lwio/etc/lwiod.reg
    lwio/etc/lwiod.reg.svcm
    lwio/etc/npfs.reg
    lwio/etc/pvfs.reg
        lwio/etc/rdr.reg
        lwio/etc/srv.reg        
        lwio/scripts/Makefile
        lwio/utils/Makefile
        lwio/common/Makefile
        lwio/ipc/Makefile
        lwio/include/Makefile
        lwio/libshareinfo/Makefile
        lwio/fuse/Makefile
        lwio/logging_r/Makefile
        lwio/server/dfs/Makefile
        lwio/server/Makefile
        lwio/server/pvfs/Makefile
        lwio/server/srvstat/Makefile
        lwio/server/iomgr/Makefile
        lwio/server/npfs/Makefile
        lwio/server/smbcommon/Makefile
        lwio/server/lwiod/Makefile
        lwio/server/rdr/Makefile
        lwio/server/srv/statistics/Makefile
        lwio/server/srv/transport/threadpool/Makefile
        lwio/server/srv/transport/Makefile
        lwio/server/srv/driver/Makefile
        lwio/server/srv/utils/Makefile
        lwio/server/srv/oem/Makefile
        lwio/server/srv/Makefile
        lwio/server/srv/elements/Makefile
        lwio/server/srv/shares/Makefile
        lwio/server/srv/shares/api/Makefile
        lwio/server/srv/shares/regshare/Makefile
        lwio/server/srv/shares/lwshare/Makefile
        lwio/server/srv/protocols/smb1/Makefile
        lwio/server/srv/protocols/Makefile
        lwio/server/srv/protocols/api/Makefile
        lwio/server/srv/protocols/smb2/Makefile
        lwio/drivers/iotest/Makefile
        lwio/drivers/Makefile
        lwio/client/Makefile
        lwio/tools/Makefile
        lwio/tools/lwio-tool/Makefile
        lwio/tools/lwio-copy/Makefile
        lwio/tools/lwio-set-log-level/Makefile
        lwio/tools/lwio-refresh/Makefile
        lwio/tools/lwio-get-log-info/Makefile
        lwio/tools/lwio-cli/Makefile
        lwio/tools/lwio-driver/Makefile
        lwio/tests/test_load/Makefile
        lwio/tests/test_pvfs/Makefile
        lwio/tests/Makefile
        lwio/tests/moonunit/Makefile
])

AC_OUTPUT
