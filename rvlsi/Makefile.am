INCLUDES = -I$(top_srcdir) -I$(R_LIB)
lib_LTLIBRARIES =  librvlsi.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
librvlsi_la_SOURCES = edif.cpp files.cpp gds.cpp rchromovlsi.cpp rgavlsi.cpp \
			rinstvlsi.cpp struct.cpp
rvlsiincludedir = $(includedir)/r/rvlsi
rvlsi_LDADD   = $(RVLSI_ADD)
rvlsiinclude_HEADERS =  edif.h files.h gds.h rchromovlsi.h rgavlsi.h rinstvlsi.h \
			rvlsiprj.h struct.h
