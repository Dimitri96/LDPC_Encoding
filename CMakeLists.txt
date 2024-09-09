cmake_minimum_required(VERSION 3.14)

project(Projektarbeit)

# Enable ExternalProject support
include(ExternalProject)

# Manually set the Python interpreter to use the one in the virtual environment
set(Python3_EXECUTABLE /home/dima/Documents/PA2/myenv/bin/python3)

# Enable Python support in CMake
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Define the paths to the Python scripts
set(PY3GPP_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/py3gpp/tests/PA_ldpc_encoder.py)
set(DATEN_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/py3gpp/tests/daten.py)
set(COMPARE_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/py3gpp/tests/compare.py)
set(TIME_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/py3gpp/tests/time.py)

# Define the paths to the executables
set(PA_LDPC_ENCODER_PATH /home/dima/Documents/PA2/srsRAN_4G/build/lib/src/phy/fec/ldpc/test/PA_ldpc_encoder)
set(PA_LDPC_ENCODER_PROJECT_PATH /home/dima/Documents/PA2/srsRAN_Project/build/tests/benchmarks/phy/upper/channel_coding/ldpc/pa_ldpc_encoder)
set(PA_TEST_EXECUTABLE_PATH /home/dima/Documents/PA2/ral-armral-24.07/build/pa_ldpc)

# Add CMake options for input and output paths
set(OUTPUT_DIR "" CACHE STRING "Directory of the input file for encoder")
set(INPUT_FILE_PATH "" CACHE STRING "Path to the input file")
set(OUTPUT_FILE_PATH "" CACHE STRING "Path to the output file")

# Add a custom target to run the Python scripts and executables in sequence with input/output file paths
add_custom_target(run_all_programs
    COMMAND ${Python3_EXECUTABLE} ${DATEN_SCRIPT_PATH} --output ${OUTPUT_DIR}
    COMMAND ${Python3_EXECUTABLE} ${PY3GPP_SCRIPT_PATH} --input ${INPUT_FILE_PATH} --output ${OUTPUT_FILE_PATH}
    COMMAND ${PA_LDPC_ENCODER_PATH} ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
    COMMAND ${PA_LDPC_ENCODER_PROJECT_PATH} ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
    COMMAND ${PA_TEST_EXECUTABLE_PATH} ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
    #COMMAND ${Python3_EXECUTABLE} ${COMPARE_SCRIPT_PATH} 
    #COMMAND ${Python3_EXECUTABLE} ${TIME_SCRIPT_PATH}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running scripts and executables with input and output paths"
)

# Define custom build directories for srsRAN_4G, srsRAN_Project, and ral-armral-24.07
set(SRSRAN_4G_BUILD_DIR /home/dima/Documents/PA2/srsRAN_4G/build)
set(SRSRAN_PROJECT_BUILD_DIR /home/dima/Documents/PA2/srsRAN_Project/build)
set(RAL_ARMRAL_BUILD_DIR /home/dima/Documents/PA2/ral-armral-24.07/build)

# Add external project for srsRAN_4G
ExternalProject_Add(
    srsRAN_4G
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/srsRAN_4G
    BINARY_DIR ${SRSRAN_4G_BUILD_DIR}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/srsRAN_4G_install
    BUILD_ALWAYS ON
)

# Add external project for srsRAN_Project
ExternalProject_Add(
    srsRAN_Project
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/srsRAN_Project
    BINARY_DIR ${SRSRAN_PROJECT_BUILD_DIR}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/srsRAN_Project_install
    BUILD_ALWAYS ON
)

# Add external project for ral-armral-24.07 with specific CMake arguments
ExternalProject_Add(
    ral_armral
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/ral-armral-24.07
    BINARY_DIR ${RAL_ARMRAL_BUILD_DIR}
    CMAKE_ARGS -DBUILD_TESTING=On -DBUILD_EXAMPLES=On -DCMAKE_INSTALL_PREFIX=/home/dima/Documents/PA2/ral-armral-24.07/install
    BUILD_ALWAYS ON
)

# Add a target to build all external projects
add_custom_target(build_all
    DEPENDS srsRAN_4G srsRAN_Project ral_armral
    COMMENT "Building all external projects"
)

# Ensure the srsRAN_4G CMakeLists.txt file defines the PA_ldpc_encoder executable
ExternalProject_Add_Step(
    srsRAN_4G
    build_encoder
    COMMAND ${CMAKE_COMMAND} --build ${SRSRAN_4G_BUILD_DIR} --target PA_ldpc_encoder
    DEPENDEES build
    COMMENT "Building PA_ldpc_encoder from srsRAN_4G"
)

# Ensure the srsRAN_Project CMakeLists.txt file defines the pa_ldpc_encoder executable
ExternalProject_Add_Step(
    srsRAN_Project
    build_project_encoder
    COMMAND ${CMAKE_COMMAND} --build ${SRSRAN_PROJECT_BUILD_DIR} --target pa_ldpc_encoder
    DEPENDEES build
    COMMENT "Building pa_ldpc_encoder from srsRAN_Project"
)

# Ensure the ral-armral-24.07 CMakeLists.txt file defines the pa_ldpc executable
ExternalProject_Add_Step(
    ral_armral
    build_pa_test
    COMMAND ${CMAKE_COMMAND} --build ${RAL_ARMRAL_BUILD_DIR} --target pa_ldpc
    DEPENDEES build
    COMMENT "Building pa_ldpc from ral-armral-24.07"
)

# Add custom targets to run the executables separately
add_custom_target(run_srsRAN_encoder
    COMMAND ${PA_LDPC_ENCODER_PATH} ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running PA_ldpc_encoder from srsRAN_4G"
)

add_custom_target(run_srsRAN_Project_encoder
    COMMAND ${PA_LDPC_ENCODER_PROJECT_PATH} ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running pa_ldpc_encoder from srsRAN_Project"
)

add_custom_target(run_ral_armral_test
    COMMAND ${PA_TEST_EXECUTABLE_PATH} ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running pa_ldpc from ral-armral-24.07"
)
