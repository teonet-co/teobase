AM_CFLAGS = -fno-strict-aliasing \
	-fPIC \
	-Wpointer-arith


libteobase_la_SOURCES = \
	teobase/socket.c \
	teobase/time.c \
	teobase/logging.c

lib_LTLIBRARIES = libteobase.la

libteobaseincludedir=$(includedir)/teobase

libteobaseinclude_HEADERS = \
	../include/teobase/platform.h \
	../include/teobase/socket.h \
	../include/teobase/time.h \
	../include/teobase/logging.h \
	../include/teobase/windows.h

libteobase_la_CFLAGS = -I$(top_srcdir)/include
libteobase_la_LDFLAGS = -version-info $(LIBRARY_CURRENT):$(LIBRARY_REVISION):$(LIBRARY_AGE)

