MACRO(DO_RVLSI_R_LIB_INCLUDE _libPath)
        INCLUDE_DIRECTORIES("${_libPath}/rvlsi")
	    INCLUDE_DIRECTORIES("${_libPath}/build")
        
        LINK_DIRECTORIES(${_libPath}/build/rvlsi ${_libPath}/rvlsi/lib)
ENDMACRO(DO_RVLSI_R_LIB_INCLUDE)
