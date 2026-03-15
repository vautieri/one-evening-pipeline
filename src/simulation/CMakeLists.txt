# src/simulation/CMakeLists.txt — JSBSim interface library
# REQ-SF-0200: JSBSim 6-DOF Flight Dynamics Integration

add_library(synthflight_simulation
    JSBSimInterface.cpp
)

target_include_directories(synthflight_simulation
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/extern/jsbsim/src
)

target_link_libraries(synthflight_simulation
    PUBLIC libJSBSim
)

target_compile_features(synthflight_simulation PUBLIC cxx_std_17)
