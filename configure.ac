#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(ibus-canna, 0.0.0, http://github.com/ftake/ibus-canna)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/engine.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
CXXFLAGS+=" -Wall"

# Checks for modules
PKG_CHECK_MODULES(IBUS, ibus-1.0)

# Checks for libraries.
AC_CHECK_LIB([canna], [jrKanjiControl], [CANNA_LIBS="-lcanna"])

ICU_LIBS=`icu-config --ldflags`
AC_SUBST(CANNA_LIBS)
AC_SUBST(ICU_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
				 canna.xml.in])
AC_OUTPUT
