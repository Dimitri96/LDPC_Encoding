import numpy as np
import argparse

def generate_random_bitstring(length=8448, one_percentage=5):
 
    # Calculate the number of 1s
    num_ones = int(length * one_percentage / 100)
    
    # Create an array of zeros and ones
    bitarray = np.zeros(length, dtype=int)
    bitarray[:num_ones] = 1
    
    # Shuffle the array to distribute 1s randomly
    np.random.shuffle(bitarray)
    
    # Convert array to string
    bitstring = ''.join(bitarray.astype(str))
    
    return bitstring

def write_bitstring_to_file(bitstring, filepath):

    with open(filepath, 'w') as file:
        file.write(bitstring)

if __name__ == "__main__":
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="Generate a random bitstring and save it to a file.")
    parser.add_argument('--output', type=str, required=True, help="Path to the output directory")
    parser.add_argument('--length', type=int, default=8448, help="Length of the bitstring (default: 8448)")
    parser.add_argument('--percentage', type=int, default=5, help="Percentage of ones in the bitstring (default: 5%)")

    args = parser.parse_args()

    # Generate the bitstring with the specified parameters
    bitstring = generate_random_bitstring(length=args.length, one_percentage=args.percentage)

    # Define the output file path
    file_path = f"{args.output}/input_test.txt"

    # Write the bitstring to the specified file
    write_bitstring_to_file(bitstring, file_path)
