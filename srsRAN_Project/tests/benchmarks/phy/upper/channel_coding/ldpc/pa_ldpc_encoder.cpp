#include "srsran/phy/upper/channel_coding/channel_coding_factories.h"
#include "srsran/support/benchmark_utils.h"
#include "srsran/support/srsran_test.h"
#include <getopt.h>
#include <random>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

namespace {
  std::string enc_type = "neon";
} // namespace

using namespace srsran;
using namespace srsran::ldpc;

int main(int argc, char** argv)
{
  // Check that the correct number of arguments is provided
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_file_path> <output_directory>\n";
    return -1;
  }

  // Get file paths from command-line arguments
  const char* input_filename = argv[1];
  const char* output_directory = argv[2];

  // Construct the output file paths using the output directory
  std::string output_filename = std::string(output_directory) + "/output_cpp.txt";
  std::string timing_results_filename = std::string(output_directory) + "/time_cpp.txt";

  // Hardcoded parameters: BG=1, LS=384, cb_len=25344
  const ldpc_base_graph_type bg = ldpc_base_graph_type::BG1;
  const lifting_size_t ls = lifting_size_t::LS384;
  const unsigned msg_length_bg = 22;

  unsigned msg_length = msg_length_bg * static_cast<int>(ls);
  unsigned cb_length = 25344;

  // Initialize encoder factory for the specified encoder type
  std::shared_ptr<ldpc_encoder_factory> encoder_factory = create_ldpc_encoder_factory_sw(enc_type);
  if (!encoder_factory) {
    fmt::print("Failed to create encoder factory.\n");
    return -1;
  }
  std::unique_ptr<ldpc_encoder> encoder = encoder_factory->create();
  if (!encoder) {
    fmt::print("Failed to create encoder.\n");
    return -1;
  }

  // Open the input file to read the message
  std::ifstream infile(input_filename);
  if (!infile.is_open()) {
    fmt::print("Failed to open input file for reading.\n");
    return -1;
  }

  // Read the message from the file into a string
  std::string input_message;
  infile >> input_message;

  // Check if the input file contains only valid characters (0s and 1s)
  for (char c : input_message) {
    if (c != '0' && c != '1') {
      fmt::print("Invalid character in input file. Only '0' and '1' are allowed.\n");
      return -1;
    }
  }

  // Ensure that the input message length matches the expected message length
  if (input_message.length() != msg_length) {
    fmt::print("Input message length does not match the expected message length ({} bits).\n", msg_length);
    return -1;
  }

  // Create the message bit buffer from the input message
  dynamic_bit_buffer data(msg_length);
  for (unsigned i_bit = 0; i_bit < msg_length; ++i_bit) {
    unsigned bit = (input_message[i_bit] == '1') ? 1 : 0;
    data.insert(bit, i_bit, 1);
  }

  // Generate codeblock buffer of size cb_length
  dynamic_bit_buffer codeblock(cb_length);

  // Set encoding configuration metadata
  srsran::codeblock_metadata::tb_common_metadata cfg_enc = {bg, ls};

  // Variables for timing
  double total_time = 0.0;
  const int num_iterations = 100;

  for (int iter = 0; iter < num_iterations; ++iter) {
    // Measure the time before encoding
    auto start = std::chrono::high_resolution_clock::now();

    // Encode the message into the codeblock
    encoder->encode(codeblock, data, cfg_enc);

    // Measure the time after encoding
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    total_time += elapsed.count();
  }

  // Calculate mean time
  double mean_time = total_time / num_iterations;

  // Open a file to save the encoded codeblock
  std::ofstream encoded_file(output_filename);
  if (!encoded_file.is_open()) {
    fmt::print("Failed to open encoded output file for writing.\n");
    return -1;
  }

  // Write the encoded codeblock to the file (byte-wise extraction)
  for (unsigned i = 0; i < codeblock.size() / 8; ++i) {
    uint8_t byte = codeblock.get_byte(i);
    for (int j = 7; j >= 0; --j) {
      encoded_file << ((byte >> j) & 1);
    }
  }
  encoded_file << "\n";

  // Close the files
  infile.close();
  encoded_file.close();

  // Write mean time to a file
  std::ofstream timing_file(timing_results_filename);
  if (!timing_file.is_open()) {
    fmt::print("Failed to open timing results file for writing.\n");
    return -1;
  }
  timing_file << "Mean time taken for encoding: " << mean_time << " seconds\n";
  timing_file.close();


  return 0;
}
