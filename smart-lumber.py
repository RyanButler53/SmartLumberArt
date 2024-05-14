# !/usr/bin/env python

import sys
import matplotlib.pyplot as plt
import matplotlib.pylab as pl
import numpy as np
import random

def plotSquare(x1,y1,x2,y2,c):
    """Plots a rectangle. x1 y1 and x2 y2 are diagonals. """
    plt.fill_between([x1,x2], [y1], [y2], color=c)
    return

# Read in filename
filename = sys.argv[1]

# Read in input from stdin and split it into groups of 8
inputStr = sys.stdin.read()
split = inputStr.split()
numSplits = len(split)//8
grouped = [split[8*group:8*group+8] for group in range(numSplits)]

# Create Figure and colors
fig,ax = plt.subplots(figsize=(10,10))
colors = pl.cm.viridis(np.linspace(0, 1, len(grouped)))

ax = plt.gca()
for i,group in enumerate(grouped):
    x1,y1,x2,y2,x3,y3,x4,y4 = [int(x) for x in group]
    
    # This line plots a line between to points
    plt.plot([x1,x2], [y1,y2],color = "black", linewidth=0.5)

    #This line fills in the corresponding square
    plotSquare(x3,y3,x4,y4,c=random.choice(colors))
    

ax.set_axis_off()

# Save figure. Could show the figure instead
# plt.show()
plt.savefig(filename)
        