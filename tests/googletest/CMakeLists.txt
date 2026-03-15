add_executable(test_math_gtest test_math_gtest.cpp)
target_link_libraries(test_math_gtest PRIVATE jsbsim_math GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test_math_gtest)

# TAR-SF-001: DAL A coverage tests for FGColumnVector3
add_executable(test_vector_dala_gtest test_vector_dala_gtest.cpp)
target_link_libraries(test_vector_dala_gtest PRIVATE jsbsim_math GTest::gtest_main)
gtest_discover_tests(test_vector_dala_gtest)

# TAR-SF-001: DAL A coverage tests for FGMatrix33
add_executable(test_matrix_dala_gtest test_matrix_dala_gtest.cpp)
target_link_libraries(test_matrix_dala_gtest PRIVATE jsbsim_math GTest::gtest_main)
gtest_discover_tests(test_matrix_dala_gtest)

add_executable(test_atmosphere_gtest test_atmosphere_gtest.cpp)
target_link_libraries(test_atmosphere_gtest PRIVATE jsbsim_atmosphere GTest::gtest_main)
gtest_discover_tests(test_atmosphere_gtest)

# TAR-SF-002: DAL A coverage tests for Atmosphere Model
add_executable(test_atmosphere_dala_gtest test_atmosphere_dala_gtest.cpp)
target_link_libraries(test_atmosphere_dala_gtest PRIVATE jsbsim_atmosphere GTest::gtest_main)
gtest_discover_tests(test_atmosphere_dala_gtest)

# TAR-SF-003: DAL A coverage tests for Terrain Engine
add_executable(test_terrain_dala_gtest test_terrain_dala_gtest.cpp)
target_link_libraries(test_terrain_dala_gtest PRIVATE synthflight_terrain GTest::gtest_main)
gtest_discover_tests(test_terrain_dala_gtest)

# TAR-SF-005: DAL A coverage tests for JSBSim Interface (REQ-SF-0200)
add_executable(test_jsbsim_interface_dala_gtest test_jsbsim_interface_dala_gtest.cpp)
target_link_libraries(test_jsbsim_interface_dala_gtest
    PRIVATE synthflight_simulation GTest::gtest_main)
target_compile_definitions(test_jsbsim_interface_dala_gtest
    PRIVATE JSBSIM_ROOT_DIR="${CMAKE_SOURCE_DIR}/extern/jsbsim")
gtest_discover_tests(test_jsbsim_interface_dala_gtest)
