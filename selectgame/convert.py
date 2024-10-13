import os

def read_binary_file(filename):
    with open(filename, 'rb') as f:
        data = f.read()
    return data

def generate_cpp_array(data, filename):
    array_name = os.path.splitext(os.path.basename(filename))[0]
    array_size = len(data)
    array_type = 'unsigned char __attribute__((aligned(4))) '  # adjust to your desired array type
    array_declaration = f"{array_type} {array_name}[] = {{"
    array_content = ', '.join(f"0x{data[i: i+1].hex()}" for i in range(0, array_size, 1))
    array_declaration += array_content + "};"
    return array_declaration

def main():
    filename = './selectgame.bin' 
    data = read_binary_file(filename)
    cpp_array = generate_cpp_array(data, filename)
    outfilename = '../VSC-PicoPAC/selectgame.h' 
    with open(outfilename, 'w') as fo:
        fo.write(cpp_array)

if __name__ == '__main__':
    main()
