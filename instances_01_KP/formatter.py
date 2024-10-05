import os

def reformat_knapPI_file(file_path, output_file_path):
    with open(file_path, 'r') as file:
        data = file.read().split()
    
    n = int(data[0])
    q = int(data[1])
    
    v = []
    w = []
    for i in range(2, 2 + 2 * n, 2):
        v.append(data[i])
        w.append(data[i + 1])
    
    with open(output_file_path, 'w') as file:
        file.write(f"{n} {q}\n")
        file.write(" ".join(v) + "\n")
        file.write(" ".join(w) + "\n")

def reformat_all_files_in_directory(directory_path):
    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)
        if os.path.isfile(file_path):
            print(f"Processing file: {file_path}")

            output_directory_path = os.path.join(directory_path, 'reformatted')
            output_file_path = os.path.join(output_directory_path, filename)
            os.makedirs(os.path.dirname(output_file_path), exist_ok=True)
            reformat_knapPI_file(file_path, output_file_path)

if __name__ == '__main__':
    INSTANCES_DIR = 'instances_01_KP\\low_dimensional'
    reformat_all_files_in_directory(INSTANCES_DIR)
