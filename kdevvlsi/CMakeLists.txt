PROJECT(kdevvlsi)
INCLUDE_DIRECTORIES(.)

SET(kdevvlsi_SOURCES
	main.cpp
	qvlsiapp.cpp
	kdevvlsi.cpp
	kappoptions.cpp
	kproject.cpp
	kheuristic.cpp
	kgaview.cpp
	kpolygonsview.cpp
)

SET(kdevvlsi_FORMS
	kdevvlsi.ui
	kappoptions.ui
	kproject.ui
	kheuristic.ui
	kgaview.ui
	kpolygonsview.ui
)


SET(kdevvlsi_HEADERS
	kdevvlsi.h
	kgaview.h
	kproject.h
	kappoptions.h
	kheuristic.h
	kpolygonsview.h
)


SET(kdevvlsi_RESOURCES
	kdevvlsi.qrc
)


QT4_WRAP_CPP(kdevvlsi_HEADERS_MOC ${kdevvlsi_HEADERS})
QT4_WRAP_UI(kdevvlsi_FORMS_HEADERS ${kdevvlsi_FORMS})
QT4_ADD_RESOURCES(kdevvlsi_RESOURCES_RCC ${kdevvlsi_RESOURCES})


INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(kdevvlsi
	${kdevvlsi_SOURCES}
	${kdevvlsi_HEADERS_MOC}
	${kdevvlsi_FORMS_HEADERS}
	${kdevvlsi_RESOURCES_RCC})

IF(WIN32)
	TARGET_LINK_LIBRARIES(kdevvlsi
						"C:/Users/Pascal/prj/r/build/rcore/librcore.a"
						"C:/Users/Pascal/prj/r/build/rcore/librmath.a"
						"C:/Users/Pascal/prj/r/build/rcore/librvlsi.a"
						"C:/Users/Pascal/prj/r/build/rcore/librcoreqt.a"
						"C:/Users/Pascal/prj/r/build/rcore/librprg.a"
						"C:/Users/Pascal/prj/r/build/rcore/librprgqt.a"
						"C:/Users/Pascal/prj/r/build/rcore/libroptimization.a"
						"C:/Users/Pascal/prj/r/build/rcore/libroptimizationqt.a"
						${QT_LIBRARIES} pthread iconv
						"C:/Program Files/MySQL/MySQL Connector C 6.1/lib/libmysql.lib"
						"C:/Program Files/MySQL/MySQL Connector C 6.1/lib/vs11/mysqlclient.lib"
						C:/curl/lib/libcurl.a C:/curl/lib/libcurldll.a)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(kdevvlsi rcore rcoreqt rprg rprgqt rmath rvlsi roptimization roptimizationqt ${QT_LIBRARIES})
ENDIF(WIN32)


########### install files ###############
# "install(TARGETS kdevvlsi ${INSTALL_TARGETS_DEFAULT_ARGS})
#install( FILES kdevvlsi.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
#install(FILES kdevvlsiui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kdevvlsi )
#install(FILES kdevvlsiui.rc  DESTINATION  /usr/share/kde4/apps/kdevvlsi )
#kde4_install_icons(${ICON_INSTALL_DIR})