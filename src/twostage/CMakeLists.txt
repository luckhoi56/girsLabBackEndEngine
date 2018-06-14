set(TWOSTAGE_SRC
        ThreeDeeFunction.cpp
        ThreeDeeFunction.h
        TwoStageFunction.cpp
        TwoStageFunction.h
        TwoStageGP.cpp
        TwoStageGP.h
        )

add_library(twostage STATIC ${TWOSTAGE_SRC})

set_target_properties(twostage PROPERTIES LINKER_LANGUAGE CXX)
