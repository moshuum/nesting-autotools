bin_PROGRAMS = nesting

nesting_SOURCES = main.cpp

nesting_LDADD = $(top_builddir)/sqlite/libsqlite3.la \
                $(top_builddir)/log4cpp/src/liblog4cpp.la

nesting_CPPFLAGS = -I$(top_builddir)/sqlite \
                   -I$(top_builddir)/log4cpp/include

nesting_LDFLAGS = -static
