import numpy as np
import sys
import matplotlib
from matplotlib import pyplot as plt
from matplotlib.gridspec import GridSpec
from matplotlib.ticker   import MaxNLocator

####################################################################################

matplotlib.rcParams['axes.formatter.limits'] = (-5, 4)

MAX_LOCATOR_NUMBER = 10
FIGURE_XSIZE = 10
FIGURE_YSIZE = 8

BACKGROUND_RGB = '#F5F5F5'
MAJOR_GRID_RGB = '#919191'

LEGEND_FRAME_ALPHA = 0.95

####################################################################################

def set_axis_properties(axes):
    axes.xaxis.set_major_locator(MaxNLocator(MAX_LOCATOR_NUMBER))
    axes.minorticks_on()
    axes.grid(which='major', linewidth=2, color=MAJOR_GRID_RGB)
    axes.grid(which='minor', linestyle=':')

def make_comb_str(in_tuple):
    if isinstance(in_tuple, str):
        return in_tuple

    out_str = in_tuple[0]
    for i in range(1, len(in_tuple)):
        out_str += "+" + in_tuple[i]

    return out_str

####################################################################################

def build_plot(title, instructions, savename):
    figure = plt.figure(figsize=(FIGURE_XSIZE, FIGURE_YSIZE), facecolor=BACKGROUND_RGB)
    gs = GridSpec(ncols=1, nrows=1, figure=figure)
    ax = figure.add_subplot(gs[0, 0])

    ax.set_xlabel("Amount of uses (relative)")
    ax.set_ylabel("Operation")
    ax.set_title(title)

    set_axis_properties(ax)

    ax.barh(range(len(instructions)), [instr[1] for instr in instructions], align='center', zorder=2)
    ax.set_yticks(range(len(instructions)), [make_comb_str(instr[0]) for instr in instructions])

    figure.tight_layout()
    figure.savefig(savename)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <filname.trace>", file=sys.stderr)
        exit(1)

    n_top_combinations = 12
    max_comb_start = 2
    max_comb_size  = 9

    trace_filename = sys.argv[1]
    instructions = []

    with open(trace_filename, "r") as trace_file:
        instructions = trace_file.readlines()

    for i in range(0, len(instructions)):
        instructions[i] = instructions[i].strip()

    unique_instr = sorted({comb : instructions.count(comb) / len(instructions) for comb in set(instructions)}.items(), \
                           key=lambda item: item[1], reverse=True)[0:n_top_combinations]

    all_combs = []

    for comb_size in range(max_comb_start, max_comb_size + 1):
        combs = []

        for i in range(0, len(instructions) - comb_size + 1):
            comb = []
            for j in range(comb_size):
                comb.append(instructions[i + j])

            combs.append(tuple(comb))

        n_combs = len(combs)

        combs_sorted = sorted({comb : combs.count(comb) / n_combs for comb in set(combs)}.items(), \
                           key=lambda item: item[1], reverse=True)[0:n_top_combinations]

        all_combs.append(combs_sorted)

    build_plot(f"10 most used combination of instructions (1 in a row)", unique_instr, "comb_1.png")

    for comb_size in range(max_comb_start, max_comb_size + 1):
        build_plot(f"10 most used combination of instructions ({comb_size} in a row)", all_combs[comb_size - max_comb_start], f"comb_{comb_size}.png")
