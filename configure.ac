# Init autoconf
AC_INIT([nesting], [0.1.0], [vicnala@gmail.com])
# Init automake
AM_INIT_AUTOMAKE([-Wall -Werror])

# We are using libtool
AC_PROG_LIBTOOL

# Configure C++ compiler
AC_PROG_CXX

# Auto config headers
AC_CONFIG_HEADERS([config.h])

# Autoconfigure those autotool packages
NESTING_LIBS="sqlite log4cpp"
AC_CONFIG_SUBDIRS([$NESTING_LIBS])

# Output files
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

