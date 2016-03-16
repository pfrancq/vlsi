#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(kdevvlsi CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("RVLSI" "RVLSI_LIB" "rvlsi")

FIND_PACKAGE(Qt4 REQUIRED)

SET(SUB_PROJECT kdevvlsi)
ADD_SUBDIRECTORY(kdevvlsi)
