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

    ax.set_xlabel("Amount of uses")
    ax.set_xlabel("Operation")
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

    trace_filename = sys.argv[1]
    instructions = []

    with open(trace_filename, "r") as trace_file:
        instructions = trace_file.readlines()

    for i in range(0, len(instructions)):
        instructions[i] = instructions[i].strip()

    n_top_combinations = 10

    comb_2 = []
    for i in range(0, len(instructions) - 1):
        comb_2.append((instructions[i], instructions[i + 1]))

    comb_3 = []
    for i in range(0, len(instructions) - 2):
        comb_3.append((instructions[i], instructions[i + 1], instructions[i + 2]))

    comb_4 = []
    for i in range(0, len(instructions) - 3):
        comb_4.append((instructions[i], instructions[i + 1], instructions[i + 2], instructions[i + 3]))

    comb_5 = []
    for i in range(0, len(instructions) - 4):
        comb_5.append((instructions[i], instructions[i + 1], instructions[i + 2], instructions[i + 3], instructions[i + 4]))

    unique_instr  = sorted({comb : instructions.count(comb) for comb in set(instructions)}.items(), \
                           key=lambda item: item[1], reverse=True)[0:n_top_combinations]
    unique_comb_2 = sorted({comb : comb_2.count(comb) for comb in set(comb_2)}.items(), \
                           key=lambda item: item[1], reverse=True)[0:n_top_combinations]
    unique_comb_3 = sorted({comb : comb_3.count(comb) for comb in set(comb_3)}.items(), \
                           key=lambda item: item[1], reverse=True)[0:n_top_combinations]
    unique_comb_4 = sorted({comb : comb_4.count(comb) for comb in set(comb_4)}.items(), \
                           key=lambda item: item[1], reverse=True)[0:n_top_combinations]
    unique_comb_5 = sorted({comb : comb_5.count(comb) for comb in set(comb_5)}.items(), \
                           key=lambda item: item[1], reverse=True)[0:n_top_combinations]

    build_plot("10 most used combination of instructions (1 in a row)", unique_instr, "comb_1.png")
    build_plot("10 most used combination of instructions (2 in a row)", unique_comb_2, "comb_2.png")
    build_plot("10 most used combination of instructions (3 in a row)", unique_comb_3, "comb_3.png")
    build_plot("10 most used combination of instructions (4 in a row)", unique_comb_4, "comb_4.png")
    build_plot("10 most used combination of instructions (5 in a row)", unique_comb_5, "comb_5.png")
