####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kdevvlsi
kdevvlsi_SOURCES = kdevvlsiview.cpp kdevvlsidoc.cpp kdevvlsi.cpp main.cpp 
kdevvlsi_LDADD   =  -lkfile -lkdeui -lkdecore $(LIB_QT) $(LIBSOCKET)


EXTRA_DIST = main.cpp kdevvlsi.cpp kdevvlsi.h kdevvlsidoc.cpp kdevvlsidoc.h kdevvlsiview.cpp kdevvlsiview.h kdevvlsiui.rc kdevvlsi.desktop kdevvlsi.xpm mini-kdevvlsi.xpm 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) kdevvlsi.desktop $(kde_appsdir)/Applications/kdevvlsi.desktop
	$(mkinstalldirs) $(kde_icondir)/medium/locolor/apps/
	$(INSTALL_DATA) kdevvlsi.xpm $(kde_icondir)/medium/locolor/apps/kdevvlsi.xpm
	$(mkinstalldirs) $(kde_icondir)/small/locolor/apps/
	$(INSTALL_DATA) mini-kdevvlsi.xpm $(kde_icondir)/small/locolor/apps/kdevvlsi.xpm

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/kdevvlsi.desktop
	-rm -f $(kde_icondir)/medium/locolor/apps/kdevvlsi.xpm
	-rm -f $(kde_icondir)/small/locolor/apps/kdevvlsi.xpm

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the library search path. 
kdevvlsi_LDFLAGS = $(all_libraries) $(KDE_RPATH)

rcdir = $(kde_datadir)/kdevvlsi
rc_DATA = kdevvlsiui.rc

messages: rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kdevvlsi.pot; \
	fi

