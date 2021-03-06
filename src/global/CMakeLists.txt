set(GLOBAL_SRC
        BetaDistMLEFunction.cpp
        BetaDistMLEFunction.h
        BinnedSampleSpace.cpp
        BinnedSampleSpace.h
        BinnedSpace.cpp
        BinnedSpace.h
        BrentModeFinder.cpp
        BrentModeFinder.h
        Distribution.cpp
        Distribution.h
        DistributionPoint.cpp
        DistributionPoint.h
        FitParametricDistribution.cpp
        FitParametricDistribution.h
        Error.h
        Function.cpp
        Function.h
        GaussianQuadratureIntegrator.cpp
        GaussianQuadratureIntegrator.h
        HookeMinFinder.cpp
        HookeMinFinder.h
        HyperIndex.cpp
        HyperIndex.h
        Interpretation.cpp
        Interpretation.h
        InterpretationSet.cpp
        InterpretationSet.h
        Matrix.cpp
        Matrix.h
        MetropolisBetaGenerator.cpp
        MetropolisBetaGenerator.h
        MetropolisGammaGenerator.cpp
        MetropolisGammaGenerator.h
        MetropolisGenerator.cpp
        MetropolisGenerator.h
        MetropolisLNGenerator.cpp
        MetropolisLNGenerator.h
        MetropolisNormalGenerator.cpp
        MetropolisNormalGenerator.h
        MetropolisSampler.cpp
        MetropolisSampler.h
        MetropolisTLNGenerator.cpp
        MetropolisTLNGenerator.h
        MinusFunction.cpp
        MinusFunction.h
        ModeFinder.cpp
        ModeFinder.h
        Model.cpp
        Model.h
        NumericalRecipes.cpp
        NumericalRecipes.h
        Parameters.cpp
        Parameters.h
        ParametricDistributionFitter.cpp
        ParametricDistributionFitter.h
        RomberIntegrator.cpp
        RomberIntegrator.h
        Sample.cpp
        Sample.h
        SampleSet.cpp
        SampleSet.h
        Sampler.cpp
        Sampler.h
        Session.cpp
        Session.h
        SimpsonIntegrator.cpp
        SimpsonIntegrator.h
        Status.cpp
        Status.h
        # SyncMutex.cpp
        # SyncMutex.h
        VegasIntegrator.cpp
        VegasIntegrator.h
        ZBrentRootFinder.cpp
        ZBrentRootFinder.h
        )

add_library(global STATIC ${GLOBAL_SRC})

set_target_properties(global PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(global
        dcdf
        ran
        )
