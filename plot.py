
import numpy as np
import matplotlib.pyplot as plt
import sys

def read_file_data(filepath):
    file = open(filepath, "r")
    lines = file.readlines()
    n = len(lines)
    data_x = np.zeros(n)
    data_y = np.zeros(n)
    label = lines[0]
    for i in range(n - 1):
        tmp = lines[i + 1].split()
        data_x[i + 1] = float(tmp[0])
        data_y[i + 1] = float(tmp[1])
    file.close()
    return label, data_x, data_y



def plot_data(filepaths):
    cnt = len(filepaths)
    for i in range (cnt):
        label, data_x, data_y = read_file_data(filepaths[i])
        plt.plot(data_x, data_y, label=label)
    
    plt.ylabel('wtime (ms)')
    plt.xlabel('n')
    plt.title("compare")
    plt.legend(loc='upper left')
    plt.show()

if len(sys.argv) < 2:
    print("wrong argument count")
    exit(1)

filepaths = []

for i in range (len(sys.argv) - 1):
    filepaths.append(sys.argv[i + 1])
    

plot_data(filepaths)

