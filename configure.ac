AC_INIT([kmlib], [0.0a], [spam@kdmurray.id.au])

AC_CONFIG_AUX_DIR(build)
AC_CONFIG_MACRO_DIR(build)

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])

AC_PROG_CC
AM_PROG_AR
AC_ISC_POSIX
AC_PROG_LIBTOOL
AM_PROG_CC_STDC


LT_INIT


AC_CONFIG_HEADERS([include/kmlib_config.h])
AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])

KMLIB_GLOBAL_CFLAGS="-std=gnu99 -O3 -Wall -Wpedantic -g -fgnu89-inline"
AC_SUBST([KMLIB_GLOBAL_CFLAGS])

AC_OUTPUT

