
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



def plot_data(filepath1, filepath2):
    label1, data_x1, data_y1 = read_file_data(filepath1)
    label2, data_x2, data_y2 = read_file_data(filepath2)
    plt.plot(data_x1, data_y1, label=label1)
    plt.plot(data_x2, data_y2, label=label2)

    plt.ylabel('wtime (ms)')
    plt.xlabel('n')
    plt.title("compare")
    plt.legend(loc='upper left')
    plt.show()

if len(sys.argv) != 3:
    print("wrong argument count")
    exit(1)

# no further error handling
filepath1 = sys.argv[1]
filepath2 = sys.argv[2]
plot_data(filepath1, filepath2)

