set(BRASSWEIBULL_SRC
        AdjustAggregateOperator.cpp
        AdjustAggregateOperator.h
        AdjustWeibullOperator.cpp
        AdjustWeibullOperator.h
        AllocatingWeibullPrior.cpp
        AllocatingWeibullPrior.h
        BRASSEngine.cpp
        BRASSEngine.h
        LinkedAdjustmentModel.cpp
        LinkedAdjustmentModel.h
        ProportionalModelOperator.cpp
        ProportionalModelOperator.h
        WeibullGroupedDataOperator.cpp
        WeibullGroupedDataOperator.h
        WeibullMeasureFunction.cpp
        WeibullMeasureFunction.h
        WeibullModelLayout.cpp
        WeibullModelLayout.h
        WeibullPrior.cpp
        WeibullPrior.h
        WeibullTestDataOperator.cpp
        WeibullTestDataOperator.h
        )

add_library(brassweibull STATIC ${BRASSWEIBULL_SRC})

set_target_properties(brassweibull PROPERTIES LINKER_LANGUAGE CXX)
