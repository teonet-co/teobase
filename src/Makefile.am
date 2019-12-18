AM_CFLAGS = -fno-strict-aliasing \
	-fPIC \
	-Wpointer-arith \
	# end of AM_CFLAGS


libteobase_la_SOURCES = \
	teobase/socket.c \
	teobase/time.c \
	teobase/logging.c \
	teobase/mutex.c \
	# end of libteobase_la_SOURCES

lib_LTLIBRARIES = libteobase.la

libteobaseincludedir=$(includedir)/teobase

libteobaseinclude_HEADERS = \
	../include/teobase/api.h \
	../include/teobase/platform.h \
	../include/teobase/socket.h \
	../include/teobase/time.h \
	../include/teobase/logging.h \
	../include/teobase/mutex.h \
	../include/teobase/windows.h \
	# end of libteobaseinclude_HEADERS

libteobase_la_CFLAGS = -I$(top_srcdir)/include
libteobase_la_LDFLAGS = -version-info $(LIBRARY_CURRENT):$(LIBRARY_REVISION):$(LIBRARY_AGE)

