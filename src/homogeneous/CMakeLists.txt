set(HOMOGENEOUS_SRC
        LogNormalBinomialDistribution.cpp
        LogNormalBinomialDistribution.h
        LogNormalPoissonDistribution.cpp
        LogNormalPoissonDistribution.h
        SimpleHomogeneous.cpp
        SimpleHomogeneous.h
        SimpleUEUpdater.cpp
        SimpleUEUpdater.h
        TabularBinomialDistribution.cpp
        TabularBinomialDistribution.h
        TabularBinomialSupport.cpp
        TabularBinomialSupport.h
        TabularDistribution.cpp
        TabularDistribution.h
        TabularIntegrator.cpp
        TabularIntegrator.h
        TabularLogNormalBinomialIntegrator.cpp
        TabularLogNormalBinomialIntegrator.h
        TabularLogNormalIntegrator.cpp
        TabularLogNormalIntegrator.h
        TabularLogNormalPoissonIntegrator.cpp
        TabularLogNormalPoissonIntegrator.h
        TabularNormalIntegrator.cpp
        TabularNormalIntegrator.h
        TabularPoissonDistribution.cpp
        TabularPoissonDistribution.h
        TabularPoissonSupport.cpp
        TabularPoissonSupport.h
        UniformBinomialDistribution.cpp
        UniformBinomialDistribution.h
        UniformPoissonDistribution.cpp
        UniformPoissonDistribution.h
        UpdatedTabularDistribution.cpp
        )

add_library(homogeneous STATIC ${HOMOGENEOUS_SRC})

set_target_properties(homogeneous PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(homogeneous
        core
        dcdf
        global
        parametric
        ran
        )
