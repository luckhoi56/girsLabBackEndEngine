set(SLATEC_SRC
        SLATECInterface.cpp
        SLATECInterface.h
        d1mach.c
        dcsevl.c
        de1.c
        dei.c
        f2c.h
        initds.c
        )

add_library(slatec STATIC ${SLATEC_SRC})

set_target_properties(slatec PROPERTIES LINKER_LANGUAGE CXX)
