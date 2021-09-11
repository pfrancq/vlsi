PROJECT(rvlsi)
INCLUDE_DIRECTORIES(.)

SET(rvlsi_TARGET_SOURCES
	struct.cpp
	files.cpp
	edif.cpp
	gds.cpp
	rchromovlsi.cpp
	rinstvlsi.cpp
	rgavlsi.cpp	
	rvlsiapp.cpp
)

SET(rvlsi_INST_INCLUDES
	struct.h
	files.h
	edif.h
	gds.h
	rchromovlsi.h
	rinstvlsi.h
	rgavlsi.h
	rvlsiapp.h
	rvlsidoc.h
)

INCLUDE_DIRECTORIES(${rvlsi_INCLUDES})

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(rvlsi STATIC ${rvlsi_TARGET_SOURCES})
    SET_TARGET_PROPERTIES(rvlsi PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(rvlsi roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(rvlsi SHARED ${rvlsi_TARGET_SOURCES})
    #ADD_SOURCE_FILE(rvlsi frontend rvlsi_INCLUDES)
    #ADD_FRONTEND(rvlsi 2 2 2.0.0 lib "r/rvlsi" rvlsi_INCLUDES)
    TARGET_LINK_LIBRARIES(rvlsi roptimization rcore rmath dl)
ENDIF(WIN32)
