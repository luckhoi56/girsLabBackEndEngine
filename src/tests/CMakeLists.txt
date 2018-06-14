set(TEST_MAIN_SRC
        tests-main.cpp
        )
set(TESTS_SRC
        SimpleHomogeneousTests.cpp
        )

foreach(testsourcefile ${TESTS_SRC})
  string(REPLACE ".cpp" "" testname ${testsourcefile})
  add_executable(${testname} ${testsourcefile} ${TEST_MAIN_SRC})
  target_link_libraries(${testname}
        global
        homogeneous
        )
  add_test(NAME ${testname} COMMAND ${testname})
endforeach(testsourcefile ${TESTS_SRC})

add_executable(tests ${TESTS_SRC} ${TEST_MAIN_SRC})

target_link_libraries(tests
        global
        homogeneous)