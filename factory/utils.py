import sys
import os

def create_c_file(binary_array, target):
    with open(f"{os.getcwd()}/{target}", 'w') as f:
        count = 0
        f.write("unsigned char wasm_binary_array[] = {\n")
        for byte in binary_array[:-1]:
            f.write(f"0x{byte.hex()}, ")
            count += 1
            if count % 20 == 0:
                f.write("\n")
        f.write(f"0x{binary_array[-1].hex()}")
        f.write("};\n")
        f.write(f"unsigned int wasm_binary_len = {len(binary_array)};")

def read_binary_file(path):
    binary_list = []
    with open(f"{os.getcwd()}/{path}", 'rb') as f:
        while (byte := f.read(1)):
            binary_list.append(byte)
    return binary_list

def main(param):
    binary_array = read_binary_file(param[1])
    create_c_file(binary_array, param[2])

if __name__ == '__main__':
    main(sys.argv)