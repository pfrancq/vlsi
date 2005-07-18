AUTOMAKE_OPTIONS        = foreign
SUBDIRS =
INCLUDES = -I$(top_srcdir) $(all_includes)
lib_LTLIBRARIES =  librvlsi.la
librvlsi_la_SOURCES = edif.cpp files.cpp gds.cpp rchromovlsi.cpp rgavlsi.cpp \
			rinstvlsi.cpp struct.cpp
rvlsiincludedir = $(includedir)/r
rvlsiinclude_HEADERS =  edif.h files.h gds.h rchromovlsi.h rgavlsi.h rinstvlsi.h \
			rvlsiprj.h struct.h
librvlsi_la_LDFLAGS     = -no-undefined -version-info 1:0:0
librvlsi_la_LIBADD = /usr/lib/libmysqlclient.la $(LIB_R)
