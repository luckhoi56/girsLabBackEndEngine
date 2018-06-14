set(DCDF_SRC
        DCDFLIBInterface.cpp
        DCDFLIBInterface.h
        cdflib.h
        dcdflib.cpp
        ipmpar.cpp
        )

add_library(dcdf STATIC ${DCDF_SRC})

set_target_properties(dcdf PROPERTIES LINKER_LANGUAGE CXX)
