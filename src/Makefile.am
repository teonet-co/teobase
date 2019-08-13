## Process this file with automake to produce Makefile.in
#

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(TEOCCL_CFLAGS)

AM_CFLAGS = \
	-g -Wall \
	-std=gnu11 \
	-fno-strict-aliasing \
	-fPIC \
	-I../include \
	-Wpointer-arith

AM_CXX = clang++
AM_CXXFLAGS = \
	-g -Wall \
	-std=c++14 \
	-fPIC \
	-I../include

## Debug info
if DEBUG
  AM_CFLAGS += -g3 -O0
  AM_CXXFLAGS += -g3 -O0
else
  AM_CFLAGS += -O2
  AM_CXXFLAGS += -O2
endif

AM_LDFLAGS = 

LIBS =

lib_LTLIBRARIES = libteobase.la

libteoccl_la_SOURCES = \
	teobase/socket.c \
	teobase/time.c \
	teobase/logging.c

libteocclincludedir=$(includedir)/libteoccl

nobase_libteocclinclude_HEADERS = \
	../include/teobase/platform.h \
	../include/teobase/socket.h \
	../include/teobase/time.h \
	../include/teobase/logging.h \
	../include/teobase/windows.h

libteoccl_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(LIBRARY_MAJOR_VERSION):0:0

libteoccl_la_LIBADD = $(TEOCCL_LIBS)
