# Library target for the JSBSim math subset (system under test)
add_library(jsbsim_math STATIC
    FGColumnVector3.cpp
    FGMatrix33.cpp
)

target_include_directories(jsbsim_math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(jsbsim_math PUBLIC cxx_std_17)
