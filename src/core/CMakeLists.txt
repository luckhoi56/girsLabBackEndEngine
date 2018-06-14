set(CORE_SRC
        BRASSError.cpp
        BRASSError.h
        BRASSStreams.cpp
        BRASSStreams.h
        ConvergenceOperator.cpp
        ConvergenceOperator.h
        DeviateGenerator.cpp
        DeviateGenerator.h
        DummyOperator.cpp
        DummyOperator.h
        GroupedData.cpp
        GroupedData.h
        JointDensityEstimate.cpp
        JointDensityEstimate.h
        NetCorrelation.cpp
        NetCorrelation.h
        NetModel.cpp
        NetModel.h
        NetOperator.cpp
        NetOperator.h
        NormalDensityOperator.cpp
        NormalDensityOperator.h
        ParamArray.cpp
        ParamArray.h
        QuickSorter.cpp
        QuickSorter.h
        SliceSampler.cpp
        SliceSampler.h
        SliceSamplerAids.cpp
        SliceSamplerAids.h
        TestConditions.cpp
        TestConditions.h
        TestData.cpp
        TestData.h
        TimedMeasure.cpp
        TimedMeasure.h
        WeibullSampleSet.cpp
        WeibullSampleSet.h
        WeibullStatus.cpp
        WeibullStatus.h
        WeightedLikeOperator.cpp
        WeightedLikeOperator.h
        )

add_library(core STATIC ${CORE_SRC})

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
