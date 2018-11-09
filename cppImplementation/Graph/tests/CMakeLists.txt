enable_testing()

add_executable(tstDependencyTree tstDependencyTree.cpp)

target_link_libraries(tstDependencyTree gtest gtest_main Graph)

add_test(NAME TestDependencyTree COMMAND tstDependencyTree)
