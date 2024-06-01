# !/usr/bin/env python

import sys
import matplotlib.pyplot as plt
import matplotlib.pylab as pl
import matplotlib as mpl
import numpy as np
import random

AVAILABLE_COLOR_MAPS = ['viridis', 'plasma', 'inferno', 'magma', 
                        'cividis','spring', 'summer', 
                        'autumn', 'winter', 'cool', 'Wistia']

def plotSquare(x1,y1,x2,y2,c):
    """Plots a rectangle. x1 y1 and x2 y2 are diagonals. """
    plt.fill_between([x1,x2], [y1], [y2], color=c)

    return

filename = sys.argv[1]
cmap = sys.argv[2]
if cmap not in AVAILABLE_COLOR_MAPS:
    cmap = 'cool'

inputStr = sys.stdin.read()
split = inputStr.split()
numSplits = len(split)//8
fig,ax = plt.subplots(figsize=(10,10))
grouped = [split[8*group:8*group+8] for group in range(numSplits)]

# Get Colors
color_x =np.linspace(0.0, 1.0, len(grouped))
colors = mpl.colormaps[cmap](color_x)

ax = plt.gca()
for i,group in enumerate(grouped):
    x1,y1,x2,y2,x3,y3,x4,y4 = [int(x) for x in group]
    
    plt.plot([x1,x2], [y1,y2],color = "black", linewidth=0.5)
    plotSquare(x3,y3,x4,y4,c=random.choice(colors))
    

ax.set_axis_off()
plt.tight_layout()

plt.savefig(filename)




        