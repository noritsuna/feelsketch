#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([PMCodeEncoderJNI], [1.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lPMCodeCommon':
AC_CHECK_LIB([PMCodeCommon], [main])
# FIXME: Replace `main' with a function in `-lPMCodeEncoder':
AC_CHECK_LIB([PMCodeEncoder], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_OUTPUT
