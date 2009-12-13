#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([PMCode], [1.0], [oss.pmcode@ci-a.com])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([PMCodeCommon/src/define.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([limits.h stddef.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memset sqrt])

AC_CONFIG_FILES([Makefile
                 PMCodeCommon/Makefile
                 PMCodeCommon/src/Makefile
                 PMCodeDecoder/Makefile
                 PMCodeDecoder/src/Makefile
                 PMCodeEncoder/Makefile
                 PMCodeEncoder/src/Makefile])
#AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_OUTPUT
