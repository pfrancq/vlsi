#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(rvlsi CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")

SET(SUB_PROJECT rvlsi)
ADD_SUBDIRECTORY(rvlsi)

###file to install
##keep this macro for galilei based project in a standard place
INSTALL(FILES rvlsimacro.cmake DESTINATION share/r)
