#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([xbattle], [6.0.1], [support@vmwishes.com])
AC_CONFIG_SRCDIR([src/xbattle.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MKDIR_P

# Checks for libraries.

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([getenv])

AC_SUBST(XLIB,${x_libraries})
AC_SUBST(XINC,${x_includes})
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
