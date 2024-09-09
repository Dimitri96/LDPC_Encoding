
def read_bits_from_python(filepath):

    with open(filepath, 'r') as file:
        bits = file.read().splitlines()  #entfernt Zeilenumbrüche
    return ''.join(bits)  # verbindet die Zeilen

def read_bits_from_c(filepath):

    with open(filepath, 'r') as file:
        bits = file.read().strip()
    return bits

def compare_bit_files(file1, file2, file3, file4):

    bits1 = read_bits_from_python(file1)
    bits2 = read_bits_from_c(file2)
    bits3 = read_bits_from_c(file3)
    bits4 = read_bits_from_c(file4)

    return bits1 == bits2 == bits3 == bits4

if __name__ == "__main__":
    file1 = "/home/dima/Documents/PA2/Output/output_python.txt"
    file2 = "/home/dima/Documents/PA2/Output/output_c.txt"
    file3 = "/home/dima/Documents/PA2/Output/output_cpp.txt"
    file4 = "/home/dima/Documents/PA2/Output/output_arm.txt"
    if compare_bit_files(file1, file2, file3, file4):
        print("Die Dateien sind identisch.")
    else:
        print("Die Dateien sind unterschiedlich.")
