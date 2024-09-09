import numpy as np
import pytest
import time
import argparse
from py3gpp.nrLDPCEncode import nrLDPCEncode

def save_to_file(filename, data):
    np.savetxt(filename, data, fmt='%d')

def read_bitstring_from_file(filepath):
    with open(filepath, 'r') as file:
        bitstring = file.read().strip()  # Read and strip any extra whitespace
    return bitstring

def bitstring_to_code_blocks(bitstring, K, F, C):
    # Calculate the number of bits per code block excluding filler bits
    bits_per_block = K - F

    # Split the bitstring into code blocks
    code_blocks = [list(map(int, bitstring[i:i + bits_per_block])) for i in range(0, len(bitstring), bits_per_block)]
    
    # Pad with filler bits if needed
    if len(code_blocks) < C:
        filler_block = [-1] * bits_per_block
        while len(code_blocks) < C:
            code_blocks.append(filler_block.copy())

    return np.array(code_blocks).T  # Transpose to match expected shape

@pytest.mark.parametrize("K", [8448])
@pytest.mark.parametrize("C", [1])
@pytest.mark.parametrize("F", [0])
@pytest.mark.parametrize("bgn", [1])
def test_nrLDPCEncode(K, C, F, bgn, input_file, output_dir):
    # Read bitstring from file
    bitstring = read_bitstring_from_file(input_file)
    
    # Convert bitstring to code blocks
    cbs = bitstring_to_code_blocks(bitstring, K, F, C)
    
    sionna_times = []
    thangaraj_times = []
    
    # Iterate 100 times for timing the encoding
    for _ in range(100):
        # Timing the encoding with 'sionna' algorithm
        start_time_sionna = time.time()
        codedcbs_sionna = nrLDPCEncode(cbs.copy(), bgn, algo='sionna')
        end_time_sionna = time.time()
        sionna_times.append(end_time_sionna - start_time_sionna)
        
        # Timing the encoding with 'thangaraj' algorithm
        start_time_thangaraj = time.time()
        codedcbs_thangaraj = nrLDPCEncode(cbs.copy(), bgn, algo='thangaraj')
        end_time_thangaraj = time.time()
        thangaraj_times.append(end_time_thangaraj - start_time_thangaraj)
    
    # Calculate the mean time for each algorithm
    mean_time_sionna = np.mean(sionna_times)
    mean_time_thangaraj = np.mean(thangaraj_times)
    
    # Define the output file paths
    output_file_path = f"{output_dir}/output_python.txt"
    timing_results_file_path = f"{output_dir}/time_python.txt"
    
    # Save the encoded data
    save_to_file(output_file_path, codedcbs_sionna)
    
    # Write the mean timing results to a file
    with open(timing_results_file_path, 'w') as file:
        file.write(f"Mean time taken for 'sionna' encoding: {mean_time_sionna:.6f} seconds\n")
        #file.write(f"Mean time taken for 'thangaraj' encoding: {mean_time_thangaraj:.6f} seconds\n")
    
    # Print the mean timing results
    #print(f"Mean time taken for 'sionna' encoding: {mean_time_sionna:.6f} seconds")
    #print(f"Mean time taken for 'thangaraj' encoding: {mean_time_thangaraj:.6f} seconds")
    
    # Assert that both encoded results are equal
    assert np.array_equal(codedcbs_sionna, codedcbs_thangaraj)

if __name__ == '__main__':
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="Run LDPC encoding tests")
    parser.add_argument('--input', type=str, required=True, help="Path to the input file")
    parser.add_argument('--output', type=str, required=True, help="Path to the output directory")

    args = parser.parse_args()

    # Parameters for the test
    bgn = 1
    C = 1
    K = 8448
    F = 0
    
    # Run the test function with arguments
    test_nrLDPCEncode(K, C, F, bgn, args.input, args.output)
