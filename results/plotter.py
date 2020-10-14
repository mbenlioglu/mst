#!/usr/bin/env python3
"""
    Created by mbenlioglu on Oct 04, 2020
"""
import argparse
import glob
import multiprocessing as mp
import os
import subprocess

import matplotlib.pyplot as plt
from tqdm import tqdm

files = sorted(glob.glob('data/*.gr'))
st_data = []
ch_data = []
num_edges = []


def work_load(_file):
    ret = subprocess.check_output([args.exec_name, _file, os.path.splitext(_file)[0] + '.extra',
                                   "results/" + os.path.splitext(os.path.basename(_file))[0] + ".out"])


def compile_prog():
    print("Compiling Program....")
    cmd = ["make"]
    if args.make_target is not None:
        cmd.append(args.make_target)
    subprocess.run(cmd)


def plot_results():
    print('-' * 30, '\nExperiments:\n')
    with tqdm(total=len(files), desc="Running " + args.exec_name) as pbar:
        with mp.Pool() as p:
            for _ in p.imap_unordered(work_load, (f for f in files)):
                pbar.update()
    for f in ["results/" + os.path.splitext(os.path.basename(x))[0] + ".out" for x in files]:
        with open(f) as ff:
            st_data.append(float(ff.readline().strip().split()[-1]))
        with open(f) as ff:
            sm, cnt = 0.0, 0
            for x in ff.readlines()[1:]:
                sm += float(x.strip().split()[-1])
                cnt += 1
            ch_data.append(sm / cnt)
    for f in files:
        with open(f) as ff:
            num_edges.append(int(ff.readline().strip().split()[-1]))

    fig, axs = plt.subplots(2, 1)
    axs[0].plot(num_edges, st_data)
    axs[0].set(xlabel='# of Edges', ylabel='Elapsed Time(s)')
    axs[0].set_title('Static MST Calculation')

    axs[1].plot(num_edges, ch_data)
    axs[1].set(xlabel='# of Edges', ylabel='Elapsed Time(s)')
    axs[1].set_title('Average time for Edge Addition')
    fig.suptitle('MST computation times with increasing edge number', fontsize=16)
    plt.tight_layout()
    plt.savefig('results/plots.png')


if __name__ == '__main__':
    mp.freeze_support()
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--make-target', default=None, type=str)
    parser.add_argument('-e', '--exec-name', default="./mst", type=str)
    args = parser.parse_args()

    compile_prog()
    plot_results()
