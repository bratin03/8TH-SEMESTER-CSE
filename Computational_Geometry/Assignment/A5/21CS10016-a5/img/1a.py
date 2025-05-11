"""
Author: Bratin Mondal
Roll No: 21CS10016
Deparment of Computer Science and Engineering
Indian Institue of Technology, Kharagpur
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import Voronoi, voronoi_plot_2d

# Define the 9 sites in a 3x3 grid
points = np.array([
    [1, 1], [1, 2], [1, 3],
    [2, 1], [2, 2], [2, 3],
    [3, 1], [3, 2], [3, 3]
])

# Compute the Voronoi diagram
vor = Voronoi(points)

# Create a plot
fig, ax = plt.subplots(figsize=(6, 6))

# Plot the Voronoi diagram with its finite vertices and regions
voronoi_plot_2d(
    vor,
    ax=ax,
    show_points=True,
    show_vertices=True,
    line_colors="blue",
    line_width=2,
    point_size=30,
)

# Automatically extract the finite vertices from the Voronoi object
finite_vertices = vor.vertices

# Plot the finite vertices in red
ax.scatter(
    finite_vertices[:, 0],
    finite_vertices[:, 1],
    color="red",
    s=100,
    label="Voronoi vertices",
)

# Annotate the finite vertices
for i, (x, y) in enumerate(finite_vertices, start=1):
    ax.text(x + 0.05, y + 0.05, f"V{i}", color="red", fontsize=12)

# Set plot limits and labels
ax.set_xlim(0, 4)
ax.set_ylim(0, 4)
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_title("Voronoi Diagram")
ax.legend()

plt.grid(True)
plt.savefig("1a.png", dpi=300)    
