PROJECT(kdevvlsi)
INCLUDE_DIRECTORIES(.)

SET (KDEVVLSI_SOURCES
	main.cpp
	kdevvlsi.cpp
	kappoptions.cpp
	kproject.cpp
	kheuristic.cpp
	kgaview.cpp
	kpolygonsview.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

KDE4_HANDLE_AUTOMOC(${KDEVVLSI_SOURCES})

KDE4_ADD_UI_FILES(KDEVVLSI_SOURCES
	kappoptions.ui
	kproject.ui
	kheuristic.ui
	kgaview.ui
	kpolygonsview.ui
)


KDE4_ADD_EXECUTABLE(kdevvlsi ${KDEVVLSI_SOURCES})
TARGET_LINK_LIBRARIES(kdevvlsi rvlsi roptimization rcore rcorekde rmath roptimizationkde ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS})


########### install files ###############
install(TARGETS kdevvlsi ${INSTALL_TARGETS_DEFAULT_ARGS})
install( FILES kdevvlsi.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install(FILES kdevvlsiui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kdevvlsi )
install(FILES kdevvlsiui.rc  DESTINATION  /usr/share/kde4/apps/kdevvlsi )
kde4_install_icons(${ICON_INSTALL_DIR})