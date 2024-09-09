# Python code to count the number of bits in the file 'ausgabe.txt'
with open('/home/dima/Documents/Projektarbeit/Output/output_arm.txt', 'r') as file_c:
    # Read the content of the file
    content_c = file_c.read()
    
    # Remove any whitespace and count the characters, which represent bits
    bit_count_c = len(content_c.strip())

print(f"Number of bits in 'encoded_output.txt': {bit_count_c}")