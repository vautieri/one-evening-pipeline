# ── libFuzzer targets ─────────────────────────────────────────────────────────
# Requires Clang with -fsanitize=fuzzer support.
# Build:  cmake -B build-fuzz -DENABLE_FUZZING=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
#         cmake --build build-fuzz
# Run:    ./build-fuzz/tests/fuzz/fuzz_vector_ops corpus/vector -max_total_time=300

set(FUZZ_FLAGS "-fsanitize=fuzzer,address,undefined -O1 -g -fno-omit-frame-pointer")

# ── Vector operations fuzzer ─────────────────────────────────────────────────
add_executable(fuzz_vector_ops fuzz_vector_ops.cpp)
target_link_libraries(fuzz_vector_ops PRIVATE jsbsim_math)
target_compile_options(fuzz_vector_ops PRIVATE -fsanitize=fuzzer,address,undefined -O1 -g -fno-omit-frame-pointer)
target_link_options(fuzz_vector_ops PRIVATE -fsanitize=fuzzer,address,undefined)

# ── Matrix operations fuzzer ─────────────────────────────────────────────────
add_executable(fuzz_matrix_ops fuzz_matrix_ops.cpp)
target_link_libraries(fuzz_matrix_ops PRIVATE jsbsim_math)
target_compile_options(fuzz_matrix_ops PRIVATE -fsanitize=fuzzer,address,undefined -O1 -g -fno-omit-frame-pointer)
target_link_options(fuzz_matrix_ops PRIVATE -fsanitize=fuzzer,address,undefined)

# ── Atmosphere fuzzer ────────────────────────────────────────────────────────
add_executable(fuzz_atmosphere fuzz_atmosphere.cpp)
target_link_libraries(fuzz_atmosphere PRIVATE jsbsim_atmosphere)
target_compile_options(fuzz_atmosphere PRIVATE -fsanitize=fuzzer,address,undefined -O1 -g -fno-omit-frame-pointer)
target_link_options(fuzz_atmosphere PRIVATE -fsanitize=fuzzer,address,undefined)
