# Init autoconf
AC_INIT([nesting], [0.1.0], [vicnala@gmail.com])
# Init automake
AM_INIT_AUTOMAKE([-Wall -Werror])
# Configure C++ compiler
AC_PROG_CXX
# Auto config headers
AC_CONFIG_HEADERS([config.h])

# Check for neede system libraries
AC_CHECK_LIB(sqlite3, main,,AC_MSG_ERROR(nesting requires sqlite3))
AC_CHECK_LIB(log4cpp, main,,AC_MSG_ERROR(nesting requires liblog4cpp))

# Output files
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

