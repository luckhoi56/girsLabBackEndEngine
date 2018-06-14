set(PARAMETRIC_SRC
        Beta0595InputMode.cpp
        Beta0595InputMode.h
        BetaDistribution.cpp
        BetaDistribution.h
        Gamma0595InputMode.cpp
        Gamma0595InputMode.h
        GammaDistribution.cpp
        GammaDistribution.h
        GenericDistribution.cpp
        GenericDistribution.h
        LogNormalDistribution.cpp
        LogNormalDistribution.h
        NormalDistribution.cpp
        NormalDistribution.h
        ParametricDistribution.cpp
        ParametricDistribution.h
        ParametricInputMode.cpp
        ParametricInputMode.h
        TrLogNormalDistribution.cpp
        TrLogNormalDistribution.h
        UniformDistribution.cpp
        UniformDistribution.h
        )

add_library(parametric STATIC ${PARAMETRIC_SRC})

set_target_properties(parametric PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(parametric
        core
        dcdf
        general
        ran
        )