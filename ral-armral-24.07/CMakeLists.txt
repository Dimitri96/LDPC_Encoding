cmake_minimum_required(VERSION 3.3)
project(armral VERSION 24.07)

if(CMAKE_VERSION VERSION_GREATER 3.4)
  # Stop CMake from automatically adding -rdynamic to linker flags because it
  # causes a warning about unused compiler options when using Clang
  cmake_policy(SET CMP0065 NEW)
endif()

# set default build type if none was specified with -DCMAKE_BUILD_TYPE=...
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to RELEASE as none was specified.")
  set(CMAKE_BUILD_TYPE
      RELEASE
      CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

option(ARMRAL_ENABLE_WEXTRA
       "Enable -Wextra when building the library and tests" OFF)
option(ARMRAL_ENABLE_WERROR
       "Enable -Werror when building the library and tests" OFF)
option(ARMRAL_ENABLE_ASAN
       "Enable AddressSanitizer when building the library and tests" OFF)
option(ARMRAL_ENABLE_EFENCE
       "Enable Electric Fence when building the library and tests" OFF)
option(ARMRAL_ENABLE_COVERAGE
       "Enable instrumentation for generating code coverage" OFF)
option(BUILD_SIMULATION "Enable building channel simulation programs" ON)
set(ARMRAL_ARCH
    NEON
    CACHE STRING "The architecture to build for ('NEON' or 'SVE2')")
set_property(CACHE ARMRAL_ARCH PROPERTY STRINGS "NEON" "SVE2")

set(ARMRAL_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixInv/arm_cmplx_hermitian_mat_inversion_f32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixInv/arm_cmplx_mat_inversion_f32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_matmul_aah_f32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_matmul_ahb_f32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_matmul_f32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_matmul_i16.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_matmul_i16_32bit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_mat_vec_mult_f32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_mat_vec_mult_i16.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_mat_vec_mult_i16_32bit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_solve_1sc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_solve_4sc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_solve_6sc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_solve_f32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixPseudoInv/arm_cmplx_pseudo_inverse_direct_f32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorDotProd/arm_cmplx_vecdot_f32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorDotProd/arm_cmplx_vecdot_f32_2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorDotProd/arm_cmplx_vecdot_i16.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorDotProd/arm_cmplx_vecdot_i16_2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorDotProd/arm_cmplx_vecdot_i16_2_32bit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorDotProd/arm_cmplx_vecdot_i16_32bit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorMult/arm_cmplx_vecmul_f32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorMult/arm_cmplx_vecmul_f32_2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorMult/arm_cmplx_vecmul_i16.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/VectorMult/arm_cmplx_vecmul_i16_2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DuRuInterface/MuLawCompression/arm_mu_law_compression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DuRuInterface/MuLawCompression/arm_mu_law_decompression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DuRuInterface/ORanBlockFloat/arm_block_float_compression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DuRuInterface/ORanBlockFloat/arm_block_float_decompression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DuRuInterface/ORanBlockScaling/arm_block_scaling_compression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DuRuInterface/ORanBlockScaling/arm_block_scaling_decompression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/Correlation/arm_correlation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_cf32_cf32_ab_t_gs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_cf32_cf32_ab_t_gu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_cf32_cf32_ac_n_gu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_cf32_cf32_ac_n_uu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_cf32_cf32_ac_t_uu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_cf32_cs16_ab_t_gu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_cf32_cs16_ac_n_uu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cf32_kernel_lookup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cs16.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cs16_cf32_cf32_ac_n_uu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cs16_cf32_cs16_ac_n_uu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_cs16_kernel_lookup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_execute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_level.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_plan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/rader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/rader_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FIR/arm_fir_filter_cf32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FIR/arm_fir_filter_cf32_decimate_2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FIR/arm_fir_filter_cs16.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FIR/arm_fir_filter_cs16_decimate_2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/Scrambling/arm_scrambling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/SeqGenerator/arm_mat_seq_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/CRC/arm_crc11.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/CRC/arm_crc16.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/CRC/arm_crc24_a.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/CRC/arm_crc24_b.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/CRC/arm_crc24_c.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/CRC/arm_crc6.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/ConvolutionalEncoder/arm_convolutional_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/ConvolutionalEncoder/arm_convolutional_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Demodulation/arm_demodulation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/LDPC/ldpc_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/LDPC/ldpc_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/LDPC/ldpc_rate_matching.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/LDPC/ldpc_rate_recovery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Modulation/arm_modulation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_crc_attachment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_crc_check.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_encoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_frozen_bits.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_rate_matching.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_rate_recovery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_subchannel_deinterleave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Polar/arm_polar_subchannel_interleave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Turbo/arm_turbo_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Turbo/arm_turbo_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Turbo/arm_turbo_rate_matching.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Turbo/arm_turbo_rate_recovery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MatrixFactorizations/SVD/arm_svd.cpp)

# Per source file compiler flag overrides/additions
if(NOT ARMRAL_OVERRIDE_COMPILE_FLAGS)
  # Enable -frename-registers for GCC release builds of
  # arm_cmplx_mat_mult_ahb_f32.c. This improves register allocation for SVE
  # instructions such as the indexed FMLA, which have a restricted range for the
  # indexed operand. A patch to improve register allocation has been accepted
  # upstream (see below) and will probably be part of GCC 14, but it is unlikely
  # to be backported to any previous releases.
  #
  # See:
  # https://github.com/gcc-mirror/gcc/commit/6d25ea520f7ed58568c9a0031409bc8e38b673f3
  # Note: We don't universally enable this flag, as in some cases it can cause
  # regressions.
  set_property(
    SOURCE
      ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicMathFun/MatrixMult/arm_cmplx_mat_mult_ahb_f32.c
    APPEND
    PROPERTY COMPILE_OPTIONS
             $<$<AND:$<CONFIG:RELEASE>,$<C_COMPILER_ID:GNU>>:-frename-registers>
  )

  if(ARMRAL_ENABLE_WERROR)
    # Disable warnings-as-errors about C-style Variable Length Arrays in FFT
    # source when using Clang++
    set_property(
      SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_execute.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/fft_plan.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/LowerPHY/FFT/rader_generator.cpp
      APPEND
      PROPERTY COMPILE_OPTIONS
               $<$<CXX_COMPILER_ID:Clang>:-Wno-error=vla-extension>)
  endif()
endif()

set(ARMRAL_UTIL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/utils/rng.cpp)

set(ARMRAL_LIB_INC
    ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/utils)

set(ARMRAL_TEST_INC
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/LDPC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UpperPHY/Turbo)

set(ARMRAL_OVERRIDE_COMPILE_FLAGS FALSE)
if(NOT CMAKE_C_FLAGS STREQUAL "")
  if(CMAKE_CXX_FLAGS STREQUAL "")
    message(
      FATAL_ERROR
        "If overriding compile flags, both CMAKE_C_FLAGS and CMAKE_CXX_FLAGS need to be set"
    )
  endif()
endif()

if(NOT CMAKE_CXX_FLAGS STREQUAL "")
  if(CMAKE_C_FLAGS STREQUAL "")
    message(
      FATAL_ERROR
        "If overriding compile flags, both CMAKE_C_FLAGS and CMAKE_CXX_FLAGS need to be set"
    )
  endif()
  set(ARMRAL_OVERRIDE_COMPILE_FLAGS TRUE)
endif()

set(ARMRAL_COMPILER_FLAGS "")
set(ARMRAL_LINKER_FLAGS "")

if(ARMRAL_ENABLE_WEXTRA)
  if(ARMRAL_OVERRIDE_COMPILE_FLAGS)
    message(
      WARNING
        "CMAKE_C_FLAGS and CMAKE_CXX_FLAGS manually specified. Ignoring option ARMRAL_ENABLE_WEXTRA"
    )
  else()
    # We have the same interfaces for Neon and SVE implementations of functions,
    # and sometimes we pass in parameters that are only used in one or the
    # other. We therefore disable warnings for unused parameters.
    set(ARMRAL_COMPILER_FLAGS ${ARMRAL_COMPILER_FLAGS} -Wextra
                              -Wno-unused-parameter)
  endif()
endif()

if(ARMRAL_ENABLE_WERROR)
  if(ARMRAL_OVERRIDE_COMPILE_FLAGS)
    message(
      WARNING
        "CMAKE_C_FLAGS and CMAKE_CXX_FLAGS manually specified. Ignoring option ARMRAL_ENABLE_WERROR"
    )
  else()
    set(ARMRAL_COMPILER_FLAGS ${ARMRAL_COMPILER_FLAGS} -Werror)
  endif()
endif()

if(ARMRAL_ENABLE_ASAN)
  if(ARMRAL_OVERRIDE_COMPILE_FLAGS)
    message(
      WARNING
        "CMAKE_C_FLAGS and CMAKE_CXX_FLAGS manually specified. Ignoring option ARMRAL_ENABLE_ASAN"
    )
  else()
    set(ARMRAL_COMPILER_FLAGS ${ARMRAL_COMPILER_FLAGS} -fsanitize=address)
    set(ARMRAL_LINKER_FLAGS ${ARMRAL_LINKER_FLAGS} -fsanitize=address)
  endif()
endif()

if(ARMRAL_ENABLE_EFENCE)
  if(ARMRAL_OVERRIDE_COMPILE_FLAGS)
    message(
      WARNING
        "CMAKE_C_FLAGS and CMAKE_CXX_FLAGS manually specified. Ignoring option ARMRAL_ENABLE_EFENCE"
    )
  else()
    set(ARMRAL_COMPILER_FLAGS ${ARMRAL_COMPILER_FLAGS} -lefence)
    set(ARMRAL_LINKER_FLAGS ${ARMRAL_LINKER_FLAGS} -lefence)
  endif()
endif()

if(ARMRAL_ENABLE_COVERAGE)
  if(ARMRAL_OVERRIDE_COMPILE_FLAGS)
    message(
      WARNING
        "CMAKE_C_FLAGS and CMAKE_CXX_FLAGS manually specified. Ignoring option ARMRAL_ENABLE_COVERAGE"
    )
  else()
    set(ARMRAL_COMPILER_FLAGS ${ARMRAL_COMPILER_FLAGS} --coverage
                              -fprofile-update=atomic)
    set(ARMRAL_LINKER_FLAGS ${ARMRAL_LINKER_FLAGS} --coverage
                            -fprofile-update=atomic)
  endif()
endif()

if(ARMRAL_SEMIHOSTING)
  if(ARMRAL_OVERRIDE_COMPILE_FLAGS)
    message(
      WARNING
        "CMAKE_C_FLAGS and CMAKE_CXX_FLAGS manually specified. Ignoring option ARMRAL_SEMIHOSTING"
    )
  else()
    set(ARMRAL_COMPILER_FLAGS ${ARMRAL_COMPILER_FLAGS} --specs=rdimon.specs)
    set(ARMRAL_LINKER_FLAGS ${ARMRAL_LINKER_FLAGS} -lc -lrdimon)
  endif()
endif()

if(CMAKE_VERSION VERSION_GREATER 3.15)
  set(JOB_POOL_CONSOLE JOB_POOL console)
endif()

if(NOT ARMRAL_OPT_FLAGS AND NOT ARMRAL_OVERRIDE_COMPILE_FLAGS)
  # If the optimization flags are already set, don't try and guess what they
  # should be.
  if(ARMRAL_ARCH STREQUAL "SVE2")
    set(ARMRAL_ARCH_TYPE "ARMRAL_ARCH_SVE=2")
    set(ARMRAL_ARCH_COMPILE_OPTIONS
        "-march=armv8.5-a+sve2+crypto+fp16"
        CACHE INTERNAL "")
  elseif(ARMRAL_ARCH STREQUAL "SVE")
    set(ARMRAL_ARCH_TYPE "ARMRAL_ARCH_SVE=1")
    set(ARMRAL_ARCH_COMPILE_OPTIONS
        "-march=armv8.2-a+sve+crypto+fp16"
        CACHE INTERNAL "")
  elseif(ARMRAL_ARCH STREQUAL "NEON")
    set(ARMRAL_ARCH_COMPILE_OPTIONS
        "-march=armv8-a+crypto"
        CACHE INTERNAL "")
  else()
    message(
      FATAL_ERROR
        "Invalid value of -DARMRAL_ARCH, should be 'NEON', 'SVE' or 'SVE2'")
  endif()
elseif(NOT ARMRAL_OVERRIDE_COMPILE_FLAGS)
  # We explicitly set the optimization flags, so just copy those. We still need
  # to set the appropriate SVE version definition
  set(ARMRAL_ARCH_COMPILE_OPTIONS ${ARMRAL_OPT_FLAGS})
  if(ARMRAL_ARCH STREQUAL "SVE2")
    set(ARMRAL_ARCH_TYPE "ARMRAL_ARCH_SVE=2")
  elseif(ARMRAL_ARCH STREQUAL "SVE")
    set(ARMRAL_ARCH_TYPE "ARMRAL_ARCH_SVE=1")
  elseif(NOT ARMRAL_ARCH STREQUAL "NEON")
    message(
      FATAL_ERROR
        "Invalid value of -DARMRAL_ARCH, should be 'NEON', 'SVE' or 'SVE2'")
  endif()
else()
  set(ARMRAL_ARCH_COMPILE_OPTIONS "")
  if(ARMRAL_ARCH STREQUAL "SVE2")
    set(ARMRAL_ARCH_TYPE "ARMRAL_ARCH_SVE=2")
  elseif(ARMRAL_ARCH STREQUAL "SVE")
    set(ARMRAL_ARCH_TYPE "ARMRAL_ARCH_SVE=1")
  elseif(NOT ARMRAL_ARCH STREQUAL "NEON")
    message(
      FATAL_ERROR
        "Invalid value of -DARMRAL_ARCH, should be 'NEON', 'SVE' or 'SVE2'")
  endif()
endif()

if(NOT ARMRAL_OVERRIDE_COMPILE_FLAGS)
  set(ARMRAL_COMPILER_FLAGS
      ${ARMRAL_COMPILER_FLAGS}
      $<$<COMPILE_LANGUAGE:C>:-Wshadow
      -Wall
      -Wcast-qual>
      $<$<COMPILE_LANGUAGE:CXX>:-Wshadow
      -Wall
      -Wcast-qual
      -fno-rtti
      -fno-exceptions
      -std=c++17>
      $<$<CONFIG:DEBUG>:-Og
      -g3
      -ggdb
      -fno-omit-frame-pointer>)
  message(STATUS "Using compilation flags: ${ARMRAL_COMPILER_FLAGS}")
else()
  # If the CMAKE_C_FLAGS is set, CMake already deals with putting this on the
  # compile line
  message(STATUS "Overriding compilation flags with manually set flags")
  message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
  message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
  set(ARMRAL_COMPILER_FLAGS "")
  set(ARMRAL_LINKER_FLAGS "")
endif()

add_library(armral ${ARMRAL_LIB_SOURCES})
target_include_directories(armral PUBLIC ${ARMRAL_LIB_INC})
target_compile_definitions(armral PUBLIC ${ARMRAL_ARCH_TYPE})
target_compile_options(armral PRIVATE ${ARMRAL_ARCH_COMPILE_OPTIONS}
                                      ${ARMRAL_COMPILER_FLAGS})
target_link_libraries(armral PRIVATE ${ARMRAL_LINKER_FLAGS})

add_library(armral_utils ${ARMRAL_UTIL_SOURCES})
target_include_directories(armral_utils PUBLIC ${ARMRAL_LIB_INC})
target_compile_definitions(armral_utils PUBLIC ${ARMRAL_ARCH_TYPE})
target_compile_options(armral_utils PRIVATE ${ARMRAL_ARCH_COMPILE_OPTIONS}
                                            ${ARMRAL_COMPILER_FLAGS})
target_link_libraries(armral_utils PRIVATE ${ARMRAL_LINKER_FLAGS})

if(ARMRAL_SEMIHOSTING)
  # when semihosting we need to pass "-DARMRAL_SEMIHOSTING" as a compiler flag,
  # so we specify the string "ARMRAL_SEMIHOSTING" rather than the CMake variable
  # ARMRAL_SEMIHOSTING
  target_compile_definitions(armral PUBLIC "ARMRAL_SEMIHOSTING")
  target_compile_definitions(armral_utils PUBLIC "ARMRAL_SEMIHOSTING")
endif()

include(GNUInstallDirs)
install(TARGETS armral DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.h")
install(FILES LICENSE.md THIRD_PARTY_LICENSES.md
        DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/armral)

if(BUILD_TESTING)
  include(CTest)

  if(NOT DEFINED BENCHMARKER_SOURCE_DIR)
    set(BENCHMARKER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
  if(NOT DEFINED BENCHMARKER_BUILD_DIR)
    set(BENCHMARKER_BUILD_DIR ${CMAKE_BINARY_DIR})
  endif()
  if(NOT DEFINED BENCHMARKER_RUNNER)
    set(BENCHMARKER_RUNNER "${BENCHMARKER_SOURCE_DIR}/bench/default_runner.py")
  endif()

  add_custom_target(
    check
    COMMAND ${CMAKE_CTEST_COMMAND}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

  add_custom_target(
    bench
    COMMAND
      ${BENCHMARKER_SOURCE_DIR}/bench/benchmarker.py ${CMAKE_SOURCE_DIR}
      ${BENCHMARKER_BUILD_DIR} --runner ${BENCHMARKER_RUNNER}
      ${JOB_POOL_CONSOLE}
    WORKING_DIRECTORY ${BENCHMARKER_BUILD_DIR})

  add_custom_target(
    bench_concurrent
    COMMAND
      ${BENCHMARKER_SOURCE_DIR}/bench/benchmarker.py ${CMAKE_SOURCE_DIR}
      ${BENCHMARKER_BUILD_DIR} --runner ${BENCHMARKER_RUNNER} --concurrent
      ${JOB_POOL_CONSOLE}
    WORKING_DIRECTORY ${BENCHMARKER_BUILD_DIR})

  add_custom_target(
    bench_excel_summary
    COMMAND
      ${BENCHMARKER_SOURCE_DIR}/bench/benchmarker.py ${CMAKE_SOURCE_DIR}
      ${BENCHMARKER_BUILD_DIR} --runner ${BENCHMARKER_RUNNER} --concurrent | tee
      ${BENCHMARKER_BUILD_DIR}/out.json
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/python/benchmark_excel_summary.py
            ${BENCHMARKER_BUILD_DIR}/out.json ${JOB_POOL_CONSOLE}
    WORKING_DIRECTORY ${BENCHMARKER_BUILD_DIR})

  set(ARMRAL_TEST_LINK_LIBRARIES armral armral_utils)

  if(STATIC_TESTING)
    set(ARMRAL_LINKER_FLAGS ${ARMRAL_LINKER_FLAGS} -static)
  endif()

  # utility function to add a test
  function(add_armral_test TEST_NAME TEST_SOURCE)
    # build the actual test executable itself
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} ${ARMRAL_TEST_LINK_LIBRARIES}
                          ${ARMRAL_LINKER_FLAGS})
    target_include_directories(${TEST_NAME} PRIVATE ${ARMRAL_TEST_INC})
    target_compile_options(${TEST_NAME} PRIVATE ${ARMRAL_COMPILER_FLAGS}
                                                ${ARMRAL_ARCH_COMPILE_OPTIONS})

    # register it as a test, set up dependencies
    add_test(NAME ${TEST_NAME} COMMAND ${ARMRAL_TEST_RUNNER}
                                       ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
    add_dependencies(check ${TEST_NAME})
  endfunction()

  # utility function to add a benchmark
  function(add_armral_bench BENCH_NAME BENCH_SOURCE)

    # build the actual bench executable itself
    add_executable(bench_${BENCH_NAME} ${BENCH_SOURCE})
    target_link_libraries(bench_${BENCH_NAME} ${ARMRAL_TEST_LINK_LIBRARIES}
                          ${ARMRAL_LINKER_FLAGS})
    target_include_directories(bench_${BENCH_NAME} PRIVATE ${ARMRAL_TEST_INC})
    target_compile_options(bench_${BENCH_NAME} PRIVATE ${ARMRAL_COMPILER_FLAGS})

    # register it as a benchmark, set up dependencies
    add_dependencies(bench bench_${BENCH_NAME})
    add_dependencies(bench_concurrent bench_${BENCH_NAME})
    add_dependencies(bench_excel_summary bench_${BENCH_NAME})

    # add target for running the benchmark
    get_filename_component(BENCH_DIR ${BENCH_SOURCE} DIRECTORY)
    add_custom_target(
      run_bench_${BENCH_NAME}
      COMMAND
        ${BENCHMARKER_SOURCE_DIR}/bench/benchmarker.py
        ${CMAKE_CURRENT_SOURCE_DIR}/${BENCH_DIR} ${BENCHMARKER_BUILD_DIR}
        --runner ${BENCHMARKER_RUNNER} --concurrent ${JOB_POOL_CONSOLE}
      WORKING_DIRECTORY ${BENCHMARKER_BUILD_DIR}
      DEPENDS bench_${BENCH_NAME})
  endfunction()

  # #add_armral_test(matrix_inv_batch test/BasicMathFun/MatrixInv/Batch/main.cpp)
  # #add_armral_test(matrix_inv_single test/BasicMathFun/MatrixInv/Single/main.cpp)
  # #add_armral_test(arm_solve
  #                 test/BasicMathFun/MatrixMult/Batch/ArmSolve/main.cpp)
  # #add_armral_test(
  #   #matrix_vector_mult_batch_16
  #   #test/BasicMathFun/MatrixMult/Batch/MatrixVectorMult16/main.cpp)
  # #add_armral_test(
  #   matrix_vector_mult_batch_32
  #   test/BasicMathFun/MatrixMult/Batch/MatrixVectorMult32/main.cpp)
  # add_armral_test(matrix_mult_16
  #                 test/BasicMathFun/MatrixMult/Single/MatrixMult16/main.cpp)
  # add_armral_test(matrix_mult_32
  #                 test/BasicMathFun/MatrixMult/Single/MatrixMult32/main.cpp)
  # add_armral_test(matrix_mult_aah_32
  #                 test/BasicMathFun/MatrixMult/Single/MatrixMultAAH32/main.cpp)
  # add_armral_test(matrix_mult_ahb_32
  #                 test/BasicMathFun/MatrixMult/Single/MatrixMultAHB32/main.cpp)
  # add_armral_test(
  #   matrix_vector_mult_single_16
  #   test/BasicMathFun/MatrixMult/Single/MatrixVectorMult16/main.cpp)
  # add_armral_test(
  #   matrix_vector_mult_single_32
  #   test/BasicMathFun/MatrixMult/Single/MatrixVectorMult32/main.cpp)
  # add_armral_test(matrix_pseudo_inv_direct
  #                 test/BasicMathFun/MatrixPseudoInv/Direct/main.cpp)
  # add_armral_test(vec_dot_16 test/BasicMathFun/VectorDotProd/VecDot16/main.cpp)
  # add_armral_test(vec_dot_16_2
  #                 test/BasicMathFun/VectorDotProd/VecDot16_2/main.cpp)
  # add_armral_test(vec_dot_16_2_32_bit
  #                 test/BasicMathFun/VectorDotProd/VecDot16_2_32bit/main.cpp)
  # add_armral_test(vec_dot_16_32_bit
  #                 test/BasicMathFun/VectorDotProd/VecDot16_32bit/main.cpp)
  # add_armral_test(vec_dot_32 test/BasicMathFun/VectorDotProd/VecDot32/main.cpp)
  # add_armral_test(vec_dot_32_2
  #                 test/BasicMathFun/VectorDotProd/VecDot32_2/main.cpp)
  # add_armral_test(vec_mul_16 test/BasicMathFun/VectorMult/VecMul16/main.cpp)
  # add_armral_test(vec_mul_16_2 test/BasicMathFun/VectorMult/VecMul16_2/main.cpp)
  # add_armral_test(vec_mul_32 test/BasicMathFun/VectorMult/VecMul32/main.cpp)
  # add_armral_test(vec_mul_32_2 test/BasicMathFun/VectorMult/VecMul32_2/main.cpp)
  # add_armral_test(mu_law_compression
  #                 test/DuRuInterface/MuLaw/Compression/main.cpp)
  # add_armral_test(mu_law_decompression
  #                 test/DuRuInterface/MuLaw/Decompression/main.cpp)
  # add_armral_test(block_float_compression
  #                 test/DuRuInterface/ORanBlockFloat/Compression/main.cpp)
  # add_armral_test(block_float_decompression
  #                 test/DuRuInterface/ORanBlockFloat/Decompression/main.cpp)
  # add_armral_test(block_scaling_compression
  #                 test/DuRuInterface/ORanBlockScaling/Compression/main.cpp)
  # add_armral_test(block_scaling_decompression
  #                 test/DuRuInterface/ORanBlockScaling/Decompression/main.cpp)
  # add_armral_test(correlation test/LowerPHY/Correlation/main.cpp)
  # add_armral_test(fft_cs16 test/LowerPHY/FFT/FFT16/main.cpp)
  # add_armral_test(fft_cf32 test/LowerPHY/FFT/FFT32/main.cpp)
  # add_armral_test(arm_fir_filter_cs16 test/LowerPHY/FIR/FIR16/main.cpp)
  # add_armral_test(arm_fir_filter_cs16_decimate_2
  #                 test/LowerPHY/FIR/FIR16Decimate2/main.cpp)
  # add_armral_test(arm_fir_filter_cf32 test/LowerPHY/FIR/FIR32/main.cpp)
  # add_armral_test(arm_fir_filter_cf32_decimate_2
  #                 test/LowerPHY/FIR/FIR32Decimate2/main.cpp)
  # add_armral_test(scrambling test/LowerPHY/Scrambling/main.cpp)
  # add_armral_test(seq_generator test/LowerPHY/SeqGenerator/main.cpp)
  # add_armral_test(crc test/UpperPHY/CRC/main.cpp)
  # add_armral_test(tail_biting_convolutional_decoding
  #                 test/UpperPHY/ConvolutionalDecoder/main.cpp)
  # add_armral_test(tail_biting_convolutional_encoding
  #                 test/UpperPHY/ConvolutionalEncoder/main.cpp)
  # add_armral_test(demodulation test/UpperPHY/Demodulation/main.cpp)
  # add_armral_test(ldpc_decoding test/UpperPHY/LDPC/Decoding/main.cpp)
  # add_armral_test(ldpc_encoding test/UpperPHY/LDPC/Encoding/main.cpp)


 # Add your test using the utility function `add_armral_test`
add_armral_test(pa_ldpc test/UpperPHY/LDPC/Encoding/pa_test.cpp)
add_armral_test(decode test/UpperPHY/LDPC/Decoding/pa_decode.cpp)

# Remove the second `add_executable` because `add_armral_test` already takes care of creating the executable.
# You do not need to add this line:
# add_executable(pa_test test/UpperPHY/LDPC/Encoding/pa_test.cpp)

# Ensure that the necessary ARM RAL libraries and test dependencies are linked properly
target_link_libraries(pa_ldpc ${ARMRAL_TEST_LINK_LIBRARIES} ${ARMRAL_LINKER_FLAGS})
target_link_libraries(decode ${ARMRAL_TEST_LINK_LIBRARIES} ${ARMRAL_LINKER_FLAGS})

# Add necessary include directories
target_include_directories(pa_ldpc PRIVATE ${ARMRAL_TEST_INC})
target_include_directories(decode PRIVATE ${ARMRAL_TEST_INC})

# Add compiler flags
target_compile_options(pa_ldpc PRIVATE ${ARMRAL_COMPILER_FLAGS} ${ARMRAL_ARCH_COMPILE_OPTIONS})
target_compile_options(decode PRIVATE ${ARMRAL_COMPILER_FLAGS} ${ARMRAL_ARCH_COMPILE_OPTIONS})

# Optionally, you can set additional properties or flags for your test.
  
  
  
  # add_armral_test(ldpc_rate_matching test/UpperPHY/LDPC/RateMatching/main.cpp)
  # add_armral_test(ldpc_rate_recovery test/UpperPHY/LDPC/RateRecovery/main.cpp)
  # add_armral_test(modulation test/UpperPHY/Modulation/main.cpp)
  # add_armral_test(polar_crc_attachment
  #                 test/UpperPHY/Polar/CrcAttachment/main.cpp)
  # add_armral_test(polar_decoder test/UpperPHY/Polar/Decoding/main.cpp)
  # add_armral_test(polar_encoder test/UpperPHY/Polar/Encoding/main.cpp)
  # add_armral_test(polar_frozen_mask test/UpperPHY/Polar/Frozen/main.cpp)
  # add_armral_test(polar_rate_matching test/UpperPHY/Polar/RateMatching/main.cpp)
  # add_armral_test(polar_rate_recovery test/UpperPHY/Polar/RateRecovery/main.cpp)
  # add_armral_test(polar_subchannel_deinterleave
  #                 test/UpperPHY/Polar/SubchannelDeinterleave/main.cpp)
  # add_armral_test(polar_subchannel_interleave
  #                 test/UpperPHY/Polar/SubchannelInterleave/main.cpp)
  # add_armral_test(turbo_decoding test/UpperPHY/Turbo/Decoding/main.cpp)
  # add_armral_test(turbo_encoding test/UpperPHY/Turbo/Encoding/main.cpp)
  # add_armral_test(turbo_rate_matching test/UpperPHY/Turbo/RateMatching/main.cpp)
  # add_armral_test(turbo_rate_recovery test/UpperPHY/Turbo/RateRecovery/main.cpp)
  # add_armral_test(svd test/MatrixFactorizations/SVD/main.cpp)

  add_armral_bench(
    matrix_inv_batch_general
    bench/BasicMathFun/MatrixInv/Batch/GeneralMatInv/NonPA/main.cpp)
  add_armral_bench(matrix_inv_batch_general_pa
                   bench/BasicMathFun/MatrixInv/Batch/GeneralMatInv/PA/main.cpp)
  add_armral_bench(
    matrix_inv_batch_hermitian
    bench/BasicMathFun/MatrixInv/Batch/HermitianMatInv/NonPA/main.cpp)
  add_armral_bench(
    matrix_inv_batch_hermitian_pa
    bench/BasicMathFun/MatrixInv/Batch/HermitianMatInv/PA/main.cpp)
  add_armral_bench(matrix_inv_single_general
                   bench/BasicMathFun/MatrixInv/Single/GeneralMatInv/main.cpp)
  add_armral_bench(matrix_inv_single_hermitian
                   bench/BasicMathFun/MatrixInv/Single/HermitianMatInv/main.cpp)
  add_armral_bench(arm_solve_1x2
                   bench/BasicMathFun/MatrixMult/Batch/ArmSolve/1x2/main.cpp)
  add_armral_bench(arm_solve_1x4
                   bench/BasicMathFun/MatrixMult/Batch/ArmSolve/1x4/main.cpp)
  add_armral_bench(arm_solve_2x2
                   bench/BasicMathFun/MatrixMult/Batch/ArmSolve/2x2/main.cpp)
  add_armral_bench(arm_solve_2x4
                   bench/BasicMathFun/MatrixMult/Batch/ArmSolve/2x4/main.cpp)
  add_armral_bench(arm_solve_4x4
                   bench/BasicMathFun/MatrixMult/Batch/ArmSolve/4x4/main.cpp)
  add_armral_bench(
    matrix_vector_mult_batch_i16_32b
    bench/BasicMathFun/MatrixMult/Batch/MatrixVectorMult16/32b/NonPA/main.cpp)
  add_armral_bench(
    matrix_vector_mult_batch_i16_32b_pa
    bench/BasicMathFun/MatrixMult/Batch/MatrixVectorMult16/32b/PA/main.cpp)
  add_armral_bench(
    matrix_vector_mult_batch_i16_64b
    bench/BasicMathFun/MatrixMult/Batch/MatrixVectorMult16/64b/NonPA/main.cpp)
  add_armral_bench(
    matrix_vector_mult_batch_i16_64b_pa
    bench/BasicMathFun/MatrixMult/Batch/MatrixVectorMult16/64b/PA/main.cpp)
  add_armral_bench(
    matrix_vector_mult_batch_f32
    bench/BasicMathFun/MatrixMult/Batch/MatrixVectorMult32/NonPA/main.cpp)
  add_armral_bench(
    matrix_vector_mult_batch_f32_pa
    bench/BasicMathFun/MatrixMult/Batch/MatrixVectorMult32/PA/main.cpp)
  add_armral_bench(
    matrix_mult_i16_32b
    bench/BasicMathFun/MatrixMult/Single/MatrixMult16/32b/main.cpp)
  add_armral_bench(
    matrix_mult_i16_64b
    bench/BasicMathFun/MatrixMult/Single/MatrixMult16/64b/main.cpp)
  add_armral_bench(
    matrix_mult_f32_2x2_iq
    bench/BasicMathFun/MatrixMult/Single/MatrixMult32/2x2/IQ/main.cpp)
  add_armral_bench(
    matrix_mult_f32_2x2
    bench/BasicMathFun/MatrixMult/Single/MatrixMult32/2x2/NonIQ/main.cpp)
  add_armral_bench(
    matrix_mult_f32_4x4_iq
    bench/BasicMathFun/MatrixMult/Single/MatrixMult32/4x4/IQ/main.cpp)
  add_armral_bench(
    matrix_mult_f32_4x4
    bench/BasicMathFun/MatrixMult/Single/MatrixMult32/4x4/NonIQ/main.cpp)
  add_armral_bench(
    matmul_f32_general
    bench/BasicMathFun/MatrixMult/Single/MatrixMult32/general/main.cpp)
  add_armral_bench(
    matrix_mult_aah_32
    bench/BasicMathFun/MatrixMult/Single/MatrixMultAAH32/main.cpp)
  add_armral_bench(
    matrix_mult_ahb_32
    bench/BasicMathFun/MatrixMult/Single/MatrixMultAHB32/main.cpp)
  add_armral_bench(
    matrix_vector_mult_i16_32b
    bench/BasicMathFun/MatrixMult/Single/MatrixVectorMult16/32bit/main.cpp)
  add_armral_bench(
    matrix_vector_mult_i16_64b
    bench/BasicMathFun/MatrixMult/Single/MatrixVectorMult16/64bit/main.cpp)
  add_armral_bench(
    matrix_vector_mult_32
    bench/BasicMathFun/MatrixMult/Single/MatrixVectorMult32/main.cpp)
  add_armral_bench(matrix_pseudo_inv_direct
                   bench/BasicMathFun/MatrixPseudoInv/Direct/main.cpp)
  add_armral_bench(vec_dot_16
                   bench/BasicMathFun/VectorDotProd/VecDot16/main.cpp)
  add_armral_bench(vec_dot_16_2
                   bench/BasicMathFun/VectorDotProd/VecDot16_2/main.cpp)
  add_armral_bench(vec_dot_16_2_32_bit
                   bench/BasicMathFun/VectorDotProd/VecDot16_2_32bit/main.cpp)
  add_armral_bench(vec_dot_16_32_bit
                   bench/BasicMathFun/VectorDotProd/VecDot16_32bit/main.cpp)
  add_armral_bench(vec_dot_32
                   bench/BasicMathFun/VectorDotProd/VecDot32/main.cpp)
  add_armral_bench(vec_dot_32_2
                   bench/BasicMathFun/VectorDotProd/VecDot32_2/main.cpp)
  add_armral_bench(vec_mul_16 bench/BasicMathFun/VectorMult/VecMul16/main.cpp)
  add_armral_bench(vec_mul_16_2
                   bench/BasicMathFun/VectorMult/VecMul16_2/main.cpp)
  add_armral_bench(vec_mul_32 bench/BasicMathFun/VectorMult/VecMul32/main.cpp)
  add_armral_bench(vec_mul_32_2
                   bench/BasicMathFun/VectorMult/VecMul32_2/main.cpp)
  add_armral_bench(mu_law_compression_14bit
                   bench/DuRuInterface/MuLaw/Compression/14bit/main.cpp)
  add_armral_bench(mu_law_compression_8bit
                   bench/DuRuInterface/MuLaw/Compression/8bit/main.cpp)
  add_armral_bench(mu_law_compression_9bit
                   bench/DuRuInterface/MuLaw/Compression/9bit/main.cpp)
  add_armral_bench(mu_law_decompression_14bit
                   bench/DuRuInterface/MuLaw/Decompression/14bit/main.cpp)
  add_armral_bench(mu_law_decompression_8bit
                   bench/DuRuInterface/MuLaw/Decompression/8bit/main.cpp)
  add_armral_bench(mu_law_decompression_9bit
                   bench/DuRuInterface/MuLaw/Decompression/9bit/main.cpp)
  add_armral_bench(
    block_float_compression_12bit
    bench/DuRuInterface/ORanBlockFloat/Compression/12bit/main.cpp)
  add_armral_bench(
    block_float_compression_14bit
    bench/DuRuInterface/ORanBlockFloat/Compression/14bit/main.cpp)
  add_armral_bench(block_float_compression_8bit
                   bench/DuRuInterface/ORanBlockFloat/Compression/8bit/main.cpp)
  add_armral_bench(block_float_compression_9bit
                   bench/DuRuInterface/ORanBlockFloat/Compression/9bit/main.cpp)
  add_armral_bench(
    block_float_decompression_12bit
    bench/DuRuInterface/ORanBlockFloat/Decompression/12bit/main.cpp)
  add_armral_bench(
    block_float_decompression_14bit
    bench/DuRuInterface/ORanBlockFloat/Decompression/14bit/main.cpp)
  add_armral_bench(
    block_float_decompression_8bit
    bench/DuRuInterface/ORanBlockFloat/Decompression/8bit/main.cpp)
  add_armral_bench(
    block_float_decompression_9bit
    bench/DuRuInterface/ORanBlockFloat/Decompression/9bit/main.cpp)
  add_armral_bench(
    block_scaling_compression_14bit
    bench/DuRuInterface/ORanBlockScaling/Compression/14bit/main.cpp)
  add_armral_bench(
    block_scaling_compression_8bit
    bench/DuRuInterface/ORanBlockScaling/Compression/8bit/main.cpp)
  add_armral_bench(
    block_scaling_compression_9bit
    bench/DuRuInterface/ORanBlockScaling/Compression/9bit/main.cpp)
  add_armral_bench(
    block_scaling_decompression_14bit
    bench/DuRuInterface/ORanBlockScaling/Decompression/14bit/main.cpp)
  add_armral_bench(
    block_scaling_decompression_8bit
    bench/DuRuInterface/ORanBlockScaling/Decompression/8bit/main.cpp)
  add_armral_bench(
    block_scaling_decompression_9bit
    bench/DuRuInterface/ORanBlockScaling/Decompression/9bit/main.cpp)
  add_armral_bench(correlation bench/LowerPHY/Correlation/main.cpp)
  add_armral_bench(fft_cs16 bench/LowerPHY/FFT/FFT16/main.cpp)
  add_armral_bench(fft_cf32 bench/LowerPHY/FFT/FFT32/main.cpp)
  add_armral_bench(arm_fir_filter_cs16 bench/LowerPHY/FIR/FIR16/main.cpp)
  add_armral_bench(arm_fir_filter_cs16_decimate_2
                   bench/LowerPHY/FIR/FIR16Decimate2/main.cpp)
  add_armral_bench(arm_fir_filter_cf32 bench/LowerPHY/FIR/FIR32/main.cpp)
  add_armral_bench(arm_fir_filter_cf32_decimate_2
                   bench/LowerPHY/FIR/FIR32Decimate2/main.cpp)
  add_armral_bench(scrambling bench/LowerPHY/Scrambling/main.cpp)
  add_armral_bench(seq_generator bench/LowerPHY/SeqGenerator/main.cpp)
  add_armral_bench(crc_11_be bench/UpperPHY/CRC/11/BigEndian/main.cpp)
  add_armral_bench(crc_11_le bench/UpperPHY/CRC/11/LittleEndian/main.cpp)
  add_armral_bench(crc_16_be bench/UpperPHY/CRC/16/BigEndian/main.cpp)
  add_armral_bench(crc_16_le bench/UpperPHY/CRC/16/LittleEndian/main.cpp)
  add_armral_bench(crc_24a_be bench/UpperPHY/CRC/24/A/BigEndian/main.cpp)
  add_armral_bench(crc_24a_le bench/UpperPHY/CRC/24/A/LittleEndian/main.cpp)
  add_armral_bench(crc_24b_be bench/UpperPHY/CRC/24/B/BigEndian/main.cpp)
  add_armral_bench(crc_24b_le bench/UpperPHY/CRC/24/B/LittleEndian/main.cpp)
  add_armral_bench(crc_24c_be bench/UpperPHY/CRC/24/C/BigEndian/main.cpp)
  add_armral_bench(crc_24c_le bench/UpperPHY/CRC/24/C/LittleEndian/main.cpp)
  add_armral_bench(crc_6_be bench/UpperPHY/CRC/6/BigEndian/main.cpp)
  add_armral_bench(crc_6_le bench/UpperPHY/CRC/6/LittleEndian/main.cpp)
  add_armral_bench(tail_biting_convolutional_decoding
                   bench/UpperPHY/ConvolutionalDecoder/main.cpp)
  add_armral_bench(tail_biting_convolutional_encoding
                   bench/UpperPHY/ConvolutionalEncoder/main.cpp)
  add_armral_bench(demodulation bench/UpperPHY/Demodulation/main.cpp)
  add_armral_bench(ldpc_decoding bench/UpperPHY/LDPC/Decoding/main.cpp)
  add_armral_bench(ldpc_encoding bench/UpperPHY/LDPC/Encoding/main.cpp)
  add_armral_bench(ldpc_rate_matching bench/UpperPHY/LDPC/RateMatching/main.cpp)
  add_armral_bench(ldpc_rate_recovery bench/UpperPHY/LDPC/RateRecovery/main.cpp)
  add_armral_bench(modulation bench/UpperPHY/Modulation/main.cpp)
  add_armral_bench(polar_decoder bench/UpperPHY/Polar/Decoding/main.cpp)
  add_armral_bench(polar_encoder bench/UpperPHY/Polar/Encoding/main.cpp)
  add_armral_bench(polar_frozen_mask bench/UpperPHY/Polar/Frozen/main.cpp)
  add_armral_bench(polar_rate_matching
                   bench/UpperPHY/Polar/RateMatching/main.cpp)
  add_armral_bench(polar_rate_recovery
                   bench/UpperPHY/Polar/RateRecovery/main.cpp)
  add_armral_bench(polar_subchannel_deinterleave
                   bench/UpperPHY/Polar/SubchannelDeinterleave/main.cpp)
  add_armral_bench(polar_subchannel_interleave
                   bench/UpperPHY/Polar/SubchannelInterleave/main.cpp)
  add_armral_bench(turbo_decoding bench/UpperPHY/Turbo/Decoding/main.cpp)
  add_armral_bench(turbo_encoding bench/UpperPHY/Turbo/Encoding/main.cpp)
  add_armral_bench(turbo_rate_matching
                   bench/UpperPHY/Turbo/RateMatching/main.cpp)
  add_armral_bench(turbo_rate_recovery
                   bench/UpperPHY/Turbo/RateRecovery/main.cpp)
  add_armral_bench(svd bench/MatrixFactorizations/SVD/main.cpp)
endif()

if(BUILD_EXAMPLES)
  add_custom_target(make_examples_dir ALL COMMAND ${CMAKE_COMMAND} -E
                                                  make_directory examples)
  add_custom_target(examples)
  add_custom_target(run_examples)
  add_dependencies(run_examples examples)

  # Any parameters after the first one will be passed as parameters to the
  # example executable when running it
  function(add_armral_example EXAMPLE_SOURCE)
    get_filename_component(EXAMPLE_EXE ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_EXE} ${EXAMPLE_SOURCE})
    add_dependencies(${EXAMPLE_EXE} make_examples_dir)
    set(EXAMPLE_OUTPUT_NAME examples/${EXAMPLE_EXE})
    set_target_properties(${EXAMPLE_EXE} PROPERTIES OUTPUT_NAME
                                                    ${EXAMPLE_OUTPUT_NAME})

    target_link_libraries(${EXAMPLE_EXE} armral m)

    add_custom_target(
      run_${EXAMPLE_EXE}
      COMMAND ${EXAMPLE_OUTPUT_NAME} ${ARGN}
      DEPENDS ${EXAMPLE_EXE})
    add_dependencies(examples ${EXAMPLE_EXE})
    add_dependencies(run_examples run_${EXAMPLE_EXE})
  endfunction()

  add_armral_example(examples/block_float_9b_example.c)
  add_armral_example(examples/fft_cf32_example.c 10)
  add_armral_example(examples/modulation_example.c)
  add_armral_example(examples/polar_example.cpp 128 100 35)
endif()

if(BUILD_SIMULATION)
  # Include simulation rules and targets This involves building dependencies
  # like AWGN library and OpenMP
  add_subdirectory(simulation)
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  add_custom_target(docs COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT})
  install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/html
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    OPTIONAL)
endif()

# Create target to uninstall the library
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)

  add_custom_target(
    uninstall COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Check that the C and C++ compilers are from the same toolchain
if(NOT CMAKE_C_COMPILER_ID STREQUAL CMAKE_CXX_COMPILER_ID)
  message(
    FATAL_ERROR
      "CXX and C compiler providers differ. Please specify the same compiler toolchain"
  )
endif()

set(COMP_ERR_MSG
    "Compilation is only supported with GNU versions 7, 8, 9, 10, \
                  11, 12, 13, or Clang versions greater than or equal to 12.0.1. \
                  If compilation fails please use one of the supported compilers."
)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_C_COMPILER_VERSION VERSION_LESS 7.1 OR CMAKE_C_COMPILER_VERSION
                                                  VERSION_GREATER 13.2)
    message(WARNING ${COMP_ERR_MSG})
  endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_C_COMPILER_VERSION VERSION_LESS 12.0.1)
    message(WARNING ${COMP_ERR_MSG})
  endif()
else()
  message(WARNING ${COMP_ERR_MSG})
endif()
