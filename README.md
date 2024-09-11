LDPC Encoding Combined Project:

This repository is a combination of four projects that have been modified to include a custom LDPC encoding function. 
This repository includes the LDPC encdoding codes, the modified CMakeLists and a CMake build system to handle all four projects. 
The following projects are combined in this repository:

1. srsRAN_4G: https://github.com/srsran/srsRAN_4G/tree/master
2. srsRAN_Project: https://github.com/srsran/srsRAN_Project/tree/main
3. py3gpp: https://github.com/catkira/py3gpp/tree/master
4. ARM RAL (Ran Acceleration Library): https://www.arm.com/resources/download/arm-ran-acceleration-library

A custom LDPC encoding code and a custom CMake file has been added to each of the four projects in the following paths:
1.srsRAN_4G: srsRAN_4G/lib/src/phy/fec/ldpc/test/PA_ldpc_encoder.c
             srsRAN_4G/lib/src/phy/fec/ldpc/test/CMakeLists.txt
             
2.srsRAN_Project: srsRAN_Project/tests/benchmarks/phy/upper/channel_coding/ldpc/pa_ldpc_encoder.cpp
                  srsRAN_Project/tests/benchmarks/phy/upper/channel_coding/ldpc/CMakeLists.txt
                  
3.py3gpp: py3gpp/tests/PA_ldpc_encoder.py

4.ARM RAL: ral-armral-24.07/test/UpperPHY/LDPC/Encoding/pa_test.cpp
           ral-armral-24.07/CMakeLists.txt

To use the LDPC encoding codes, follow these steps:

Step 1. Clone the Four Projects

You need to clone all four original repositories first:
git clone https://github.com/srsran/srsRAN_4G.git
git clone https://github.com/srsran/srsRAN_Project.git
git clone https://github.com/catkira/py3gpp.git

Step 2. Build the Projects

Follow the individual build steps for each project:
For srsRAN_4G and srsRAN_Project, follow their respective build instructions found in their README files.
For py3gpp, make sure to install all necessary dependencies and follow its build steps.
For ARM RAL, follow the provided documentation on the ARM website: https://developer.arm.com/documentation/102249/2407/Tutorials/Get-started-with-Arm-RAN-Acceleration-Library--ArmRAL-

Step 3. Add the LDPC Encoding Code

After building each of the projects, copy the provided LDPC encoder code files from this repository to the corresponding paths:
srsRAN_4G: Copy PA_ldpc_encoder.c to srsRAN_4G/lib/src/phy/fec/ldpc/test/
srsRAN_Project: Copy pa_ldpc_encoder.cpp to srsRAN_Project/tests/benchmarks/phy/upper/channel_coding/ldpc/
py3gpp: Copy PA_ldpc_encoder.py to py3gpp/tests/
ARM RAL: Copy pa_test.cpp to ral-armral-24.07/test/UpperPHY/LDPC/Encoding/

Step 4. Create a Build Directory

After adding the LDPC encoder codes, create a build directory that will hold and build all four projects using the provided CMake configuration in this repository.
      
      1. Create a new directory for building and for the output files to store in:
            mkdir build

            mkdir output
         
            cd build
         
      2. Run the CMake command to configure the build. You need to specify the input and output directories:
            cmake -DOUTPUT_DIR=input_file_directory -DINPUT_FILE_PATH=input_file_path -DOUTPUT_FILE_PATH=output_directory ..

Step 5. Compile and Run

Once the CMake configuration is complete, compile and run the entire project:

      1. Compile the project using:
               make -j $(nproc)
         
      2. Run all the programs with:
               make run_all_programs
         
This will build and execute all the LDPC encoder functions added to each of the four projects.

Step 6. Compare the Results

Once all LDPC codes have been run, modify and execute the comparison scripts to compare the results of the four projects:
Open the compare.py and time.py files and update the paths to the result files stored in the output/ directory. 
  1. compare.py, to see if the encoded files are all the same 
  2. time.py, to see which of the four implementations is the fastest



