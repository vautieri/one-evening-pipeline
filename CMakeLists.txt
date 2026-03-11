cmake_minimum_required(VERSION 3.16)
project(jsbsim_math_unit_tests
    VERSION 1.0.0
    DESCRIPTION "Unit-test demonstration: JSBSim math library with 5 C++ frameworks"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Options: enable / disable individual frameworks ──────────────────────────
option(ENABLE_GTEST    "Build Google Test suite"  ON)
option(ENABLE_CATCH2   "Build Catch2 suite"       ON)
option(ENABLE_DOCTEST  "Build doctest suite"      ON)
option(ENABLE_BOOSTTEST "Build Boost.Test suite"  OFF)  # Requires local Boost
option(ENABLE_CPPUNIT  "Build CppUnit suite"      OFF)  # Requires local CppUnit
option(ENABLE_FUZZING  "Build libFuzzer targets"   OFF)  # Requires Clang

# ── The libraries under test ─────────────────────────────────────────────────
add_subdirectory(src/math)
add_subdirectory(src/models)

# ── Test suites ──────────────────────────────────────────────────────────────
enable_testing()
add_subdirectory(tests)

message(STATUS "")
message(STATUS "═══════════════════════════════════════════════════════════")
message(STATUS "  Framework status:")
message(STATUS "    Google Test . . ${ENABLE_GTEST}")
message(STATUS "    Catch2  . . . . ${ENABLE_CATCH2}")
message(STATUS "    doctest . . . . ${ENABLE_DOCTEST}")
message(STATUS "    Boost.Test  . . ${ENABLE_BOOSTTEST}")
message(STATUS "    CppUnit . . . . ${ENABLE_CPPUNIT}")
message(STATUS "    libFuzzer . . . ${ENABLE_FUZZING}")
message(STATUS "═══════════════════════════════════════════════════════════")
message(STATUS "")
