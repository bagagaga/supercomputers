import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
from matplotlib import style
import subprocess
import time
import threading
import os


if __name__ == '__main__':
    fig, ax = plt.subplots(3, figsize=(7, 15))
    energy_k = []
    energy_p = []
    energy_t = []

    with open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_k.txt", 'r') as f:
        for e in f.readlines():
            energy_k.append(float(e.replace('\n', '')))
    with open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_p.txt", 'r') as f:
        for e in f.readlines():
            energy_p.append(float(e.replace('\n', '')))
    with open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_t.txt", 'r') as f:
        for e in f.readlines():
            energy_t.append(float(e.replace('\n', '')))

    ax[0].plot(range(len(energy_k)), energy_k)
    ax[0].set_ylabel('Kinetic energy')
    ax[0].grid()

    ax[1].plot(range(len(energy_p)), energy_p)
    ax[1].set_ylabel('Potential energy')
    ax[1].grid()

    ax[2].plot(range(len(energy_t)), energy_t)
    ax[2].set_ylabel('Total energy')
    ax[2].grid()

    plt.tight_layout()
    # plt.savefig("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\two_particles")
    plt.show()

    print("Done!")

