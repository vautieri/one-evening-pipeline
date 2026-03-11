# Library target for the JSBSim atmosphere extract (system under test)
add_library(jsbsim_atmosphere STATIC
    FGStandardAtmosphere1976.cpp
)

target_include_directories(jsbsim_atmosphere PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(jsbsim_atmosphere PUBLIC cxx_std_17)
