set(RAN_SRC
        RANLIBInterface.cpp
        RANLIBInterface.h
        com.cpp
        linpack.cpp
        ranlib.cpp
        ranlib.h
        )

add_library(ran STATIC ${RAN_SRC})

set_target_properties(ran PROPERTIES LINKER_LANGUAGE CXX)
