#include "armral.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono> // Include chrono for timing

std::vector<uint8_t> convert_to_bytes(const std::string& input_message) {
    size_t num_bytes = (input_message.size() + 7) / 8; // Number of bytes needed 
    std::vector<uint8_t> input_data(num_bytes, 0);     // Initialize vector with zeroed bytes

    // Process the input message in chunks of 8 bits
    for (size_t i = 0; i < input_message.size(); ++i) {
        size_t byte_index = i / 8;    // Determine the current byte index
        size_t bit_index = i % 8;     // Determine the position of the bit within the byte

        if (input_message[i] == '1') {
            input_data[byte_index] |= (1 << (7 - bit_index)); // Set the bit in the byte
        }
    }

    return input_data;
}

int main(int argc, char** argv) {
    // Check that the correct number of arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file_path> <output_directory>" << std::endl;
        return 1;
    }

    // Set file paths from command-line arguments
    std::string input_file_path = argv[1];
    std::string output_dir = argv[2];

    // Construct the output file paths using the output directory
    std::string output_file_path = output_dir + "/output_arm.txt";
    std::string timing_file_path = output_dir + "/time_arm.txt";
    
    // Open the input file to read the binary message
    std::ifstream infile(input_file_path);
    if (!infile.is_open()) {
        std::cerr << "Failed to open the input file: " << input_file_path << std::endl;
        return 1;
    }

    // Read the entire input message from the file into a string
    std::string input_message;
    infile >> input_message;

    // Check if the input file contains only valid characters (0s and 1s)
    for (char c : input_message) {
        if (c != '0' && c != '1') {
            std::cerr << "Invalid character in input file. Only '0' and '1' are allowed." << std::endl;
            return 1;
        }
    }

    infile.close();

    // Convert the input message string to a vector of bytes
    std::vector<uint8_t> input_data = convert_to_bytes(input_message);

    // Encoding parameters
    uint32_t lifting_size = 384; // Example lifting size, change as per your needs
    armral_ldpc_graph_t graph_type = LDPC_BASE_GRAPH_1; // Hardcoded base graph type
    uint32_t len_filler_bits = 0; // Example filler bits

    // Determine output buffer size based on the base graph and lifting size
    uint32_t codeblock_size = (graph_type == LDPC_BASE_GRAPH_1) ? 66 * lifting_size : 50 * lifting_size;
    std::vector<uint8_t> encoded_data(codeblock_size / 8); // Output buffer for encoded data

    // Variables for timing
    double total_time = 0.0;
    const int num_iterations = 100;

    for (int iter = 0; iter < num_iterations; ++iter) {
        // Measure the time before encoding
        auto start = std::chrono::high_resolution_clock::now();

        // Perform the encoding using the provided armral_ldpc_encode_block function
        armral_status status = armral_ldpc_encode_block(
            input_data.data(), // Input data as bytes
            graph_type,        // Hardcoded base graph type
            lifting_size,      // Lifting size
            len_filler_bits,   // Number of filler bits
            encoded_data.data() // Output data buffer
        );

        // Check if the encoding was successful
        if (status != ARMRAL_SUCCESS) {
            std::cerr << "Encoding failed with status code: " << status << std::endl;
            return 1;
        }

        // Measure the time after encoding
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        total_time += elapsed.count();
    }

    // Calculate mean time
    double mean_time = total_time / num_iterations;

    // Open a file to save the encoded codeblock
    std::ofstream output_file(output_file_path);
    if (!output_file) {
        std::cerr << "Failed to open the output file: " << output_file_path << std::endl;
        return 1;
    }

    // Write the encoded codeblock to the file bit-wise as a continuous stream
    for (unsigned i = 0; i < encoded_data.size(); ++i) {
        uint8_t byte = encoded_data[i];
        for (int bit = 7; bit >= 0; --bit) {
            output_file << ((byte >> bit) & 1);
        }
    }

    output_file << "\n"; // Write a newline after the codeblock
    output_file.close();

    // Write mean time to a file
    std::ofstream timing_file(timing_file_path);
    if (!timing_file.is_open()) {
        std::cerr << "Failed to open timing results file for writing: " << timing_file_path << std::endl;
        return 1;
    }
    timing_file << "Mean time taken for encoding: " << mean_time << " seconds\n";
    timing_file.close();

    //std::cout << "Encoded data has been written to " << output_file_path << " in bit-wise format." << std::endl;
    //std::cout << "Mean time taken for encoding: " << mean_time << " seconds" << std::endl;

    return 0;
}
