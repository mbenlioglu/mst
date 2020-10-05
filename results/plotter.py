"""
    Created by mbenlioglu on Oct 04, 2020
"""
import argparse
import glob
import os
import subprocess

import matplotlib.pyplot as plt
from tqdm import tqdm

files = glob.glob('data/*.gr')
st_data = []
ch_data = []
num_edges = []


def compile_prog():
    print("Compiling Program....")
    cmd = ["make"]
    if args.make_target is not None:
        cmd.append(args.make_target)
    subprocess.run(cmd)


def plot_results():
    print('-' * 30, '\nExperiments:\n')
    with tqdm(total=len(files), desc="Running " + args.exec_name) as pbar:
        for f in files:
            ret = subprocess.run([args.exec_name, f, os.path.splitext(f)[0],
                                  "results/" + os.path.splitext(os.path.basename(f))[0] + ".out"]).returncode
            pbar.update(1)
    for f in ["results/" + os.path.splitext(os.path.basename(x))[0] + ".out" for x in files]:
        with open(f) as ff:
            st_data.append(float(x.strip().split()[-1]) for x in ff.readline())
        with open(f) as ff:
            ch_data.append([float(x.strip().split()[-1]) for x in ff.readlines()[1:]])
    for f in files:
        with open(f) as ff:
            num_edges.append(int(ff.readline().strip().split()[-1]))

    fig, axs = plt.subplots(2, 1)
    axs[0].plot(num_edges, st_data)
    axs[0].set(xlabel='# of Edges', ylabel='Elapsed Time(s)')
    axs[0].set_title('Static MST Calculation')

    axs[1].plot(num_edges, ch_data)
    axs[1].set(xlabel='# of Edges', ylabel='Elapsed Time(s)')
    axs[1].set_title('1000 Edge Addition')
    plt.tight_layout()
    plt.savefig('results/plots.pdf')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--make-target', default=None, type=str)
    parser.add_argument('-r', '--repeat', default=3, type=int)
    parser.add_argument('-e', '--exec-name', default="./mst", type=str)
    args = parser.parse_args()

    compile_prog()
    plot_results()
