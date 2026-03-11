# ── Test framework orchestrator ───────────────────────────────────────────────
# Each subdirectory fetches (or finds) its framework and links against
# the jsbsim_math library built from src/math.

include(FetchContent)

# ═══════════════════════════════════════════════════════════════════════════════
# 1) Google Test
# ═══════════════════════════════════════════════════════════════════════════════
if(ENABLE_GTEST)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(googletest)
endif()

# ═══════════════════════════════════════════════════════════════════════════════
# 2) Catch2
# ═══════════════════════════════════════════════════════════════════════════════
if(ENABLE_CATCH2)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    add_subdirectory(catch2)
endif()

# ═══════════════════════════════════════════════════════════════════════════════
# 3) doctest
# ═══════════════════════════════════════════════════════════════════════════════
if(ENABLE_DOCTEST)
    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG        v2.4.11
    )
    FetchContent_MakeAvailable(doctest)
    add_subdirectory(doctest)
endif()

# ═══════════════════════════════════════════════════════════════════════════════
# 4) Boost.Test  (requires system-installed Boost)
# ═══════════════════════════════════════════════════════════════════════════════
if(ENABLE_BOOSTTEST)
    find_package(Boost REQUIRED COMPONENTS unit_test_framework)
    add_subdirectory(boosttest)
endif()

# ═══════════════════════════════════════════════════════════════════════════════
# 5) CppUnit  (requires system-installed CppUnit)
# ═══════════════════════════════════════════════════════════════════════════════
if(ENABLE_CPPUNIT)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CPPUNIT REQUIRED IMPORTED_TARGET cppunit)
    add_subdirectory(cppunit)
endif()

# ═══════════════════════════════════════════════════════════════════════════════
# 6) libFuzzer  (requires Clang)
# ═══════════════════════════════════════════════════════════════════════════════
if(ENABLE_FUZZING)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "libFuzzer requires Clang. Set -DCMAKE_CXX_COMPILER=clang++")
    endif()
    add_subdirectory(fuzz)
endif()
