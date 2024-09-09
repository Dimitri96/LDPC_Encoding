#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>  // Include for timing

#include "srsran/phy/fec/ldpc/ldpc_common.h"
#include "srsran/phy/fec/ldpc/ldpc_encoder.h"
#include "srsran/phy/utils/debug.h"
#include "srsran/phy/utils/vector.h"

int main(int argc, char* argv[]) {
    // Check for the correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file_path> <output_directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Assign command-line arguments to file path variables
    const char* input_filename = argv[1];
    const char* output_directory = argv[2];

    // Construct the output file paths using the output directory
    char output_filename[1024];
    char timing_results_filename[1024];

    snprintf(output_filename, sizeof(output_filename), "%s/output_c.txt", output_directory);
    snprintf(timing_results_filename, sizeof(timing_results_filename), "%s/time_c.txt", output_directory);
    
    // Hardcoded parameters
    srsran_basegraph_t base_graph = BG1;  // Hardcoded base graph (BG1)
    int lift_size = 384;  // Hardcoded lifting size

    // Initialize LDPC encoder
    srsran_ldpc_encoder_t encoder;
    if (srsran_ldpc_encoder_init(&encoder, SRSRAN_LDPC_ENCODER_C, base_graph, lift_size) != 0) {
        perror("Encoder initialization failed");
        exit(EXIT_FAILURE);
    }

    // Open the input file
    FILE* input_file = fopen(input_filename, "r");
    if (!input_file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    // Read the input file into a buffer
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, file_size, input_file);
    fclose(input_file);
    buffer[file_size] = '\0';

    // Prepare the message
    size_t finalK = encoder.liftK;
    size_t finalN = encoder.liftN - 2 * lift_size;

    // Count valid binary characters ('0' or '1') in the input file
    size_t valid_bits = 0;
    for (long i = 0; i < file_size; i++) {
        if (buffer[i] == '0' || buffer[i] == '1') {
            valid_bits++;
        }
    }

    size_t num_messages = valid_bits / finalK;  // Number of messages

    uint8_t* messages = (uint8_t*)malloc(finalK * num_messages);
    uint8_t* codewords = (uint8_t*)malloc(finalN * num_messages);
    if (!messages || !codewords) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Convert the message to binary format, ignoring other characters
    size_t msg_index = 0;
    for (long i = 0; i < file_size; i++) {
        if (buffer[i] == '0' || buffer[i] == '1') {
            messages[msg_index++] = buffer[i] - '0';
        }
    }

    // Variables for timing
    double total_time = 0.0;
    const int num_iterations = 100;

    for (int iter = 0; iter < num_iterations; iter++) {
        // Measure the time before encoding
        clock_t start_time = clock();

        // Perform the encoding
        for (size_t i = 0; i < num_messages; i++) {
            srsran_ldpc_encoder_encode_rm(&encoder, messages + i * finalK, codewords + i * finalN, finalK, finalN);
        }

        // Measure the time after encoding
        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        total_time += elapsed_time;
    }

    // Calculate mean time
    double mean_time = total_time / num_iterations;

    // Write the encoded results to the output file
    FILE* output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < finalN * num_messages; i++) {
        fprintf(output_file, "%d", codewords[i]);
        if ((i + 1) % finalN == 0) {
            fprintf(output_file, "\n");  // New line after each codeword
        }
    }

    fclose(output_file);

    // Write mean time to a file
    FILE* timing_file = fopen(timing_results_filename, "w");
    if (!timing_file) {
        perror("Failed to open timing results file");
        exit(EXIT_FAILURE);
    }
    fprintf(timing_file, "Mean time taken for encoding: %.6f seconds\n", mean_time);
    fclose(timing_file);

    // Free allocated memory
    free(buffer);
    free(messages);
    free(codewords);
    srsran_ldpc_encoder_free(&encoder);


    return 0;
}
