#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([emc], [0.0.1], [ccarvalho@expertisesolutions.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

#EFL_COMPILER_FLAG([-Wall -Wextra -Wpointer-arith -Wno-missing-field-initializers -fvisibility=hidden -fdata-sections -ffunction-sections])
#EFL_LINKER_FLAG([-fvisibility=hidden -fdata-sections -ffunction-sections -Wl,--gc-sections -fno-strict-aliasing -Wl,--as-needed -Wl,--no-copy-dt-needed-entries])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET

AC_CHECK_HEADER_STDBOOL
AC_CHECK_FUNCS([memset])

# Checks for libraries.
# Check for EFL
PKG_CHECK_MODULES([EMC],
[
 elementary >= 1.11.0
 eo >= 1.11.0
 eo-cxx >= 1.11.0
 eina-cxx >= 1.11.0
 edje-cxx >= 1.11.0
 evas-cxx >= 1.11.0
 ecore-cxx >= 1.11.0
 edje-cxx >= 1.11.0
 eolian-cxx >= 1.11.0
])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
