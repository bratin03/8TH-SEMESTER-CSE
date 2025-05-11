"""
Author: Bratin Mondal
Roll No: 21CS10016
Department of Computer Science and Engineering
Indian Institute of Technology, Kharagpur
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from scipy.spatial import Voronoi, voronoi_plot_2d


def point_in_box(point, box_min, box_max):
    """Check if a point is inside a bounding box."""
    return np.all(point >= box_min) and np.all(point <= box_max)


def plot_voronoi(
    points: np.ndarray, save_path: str = "Vor.png", bounding_square: tuple = None
) -> None:
    """
    Plots a Voronoi diagram for the given set of points and prints diagram details.

    Parameters:
        points (np.ndarray): An Nx2 array of coordinates for the Voronoi sites.
        save_path (str): Path to save the generated plot.
        bounding_square (tuple or None): If provided, should be (center_x, center_y, side_length)
                                          to bound the diagram inside an axis-aligned square.
    """
    # Compute the Voronoi diagram
    vor = Voronoi(points)

    # Diagram details
    num_faces = len(points)
    num_vertices = len(vor.vertices)
    num_infinite_edges = 0
    num_edges_in_box = 0
    finite_edges = []

    # Determine bounding box corners
    if bounding_square:
        cx, cy, length = bounding_square
        half_len = length / 2
        box_min = np.array([cx - half_len, cy - half_len])
        box_max = np.array([cx + half_len, cy + half_len])
    else:
        box_min = np.array([-np.inf, -np.inf])
        box_max = np.array([np.inf, np.inf])

    # Analyze edges
    for point_indices in vor.ridge_vertices:
        if -1 in point_indices:
            num_infinite_edges += 1
            continue  # Skip drawing infinite edges
        p1, p2 = vor.vertices[point_indices]
        if point_in_box(p1, box_min, box_max) or point_in_box(p2, box_min, box_max):
            num_edges_in_box += 1
            finite_edges.append((p1, p2))

    # Print VD statistics
    print("Voronoi Diagram Details:")
    print(f"  Number of faces            : {num_faces}")
    print(f"  Number of vertices         : {num_vertices}")
    print(f"  Number of finite edges     : {len(finite_edges)}")
    print(f"  Number of infinite edges   : {num_infinite_edges}")
    print(f"  Total edges in bounding box: {num_edges_in_box+num_infinite_edges}")

    # Plot
    fig, ax = plt.subplots(figsize=(6, 6))
    voronoi_plot_2d(
        vor,
        ax=ax,
        show_points=True,
        show_vertices=True,
        line_colors="blue",
        line_width=2,
        point_size=30,
    )

    # Annotate vertices
    for i, (x, y) in enumerate(vor.vertices, start=1):
        ax.text(x + 0.05, y + 0.05, f"V{i}", color="red", fontsize=12)

    # Annotate edges
    for idx, (p1, p2) in enumerate(finite_edges, start=1):
        mid_x, mid_y = (p1 + p2) / 2
        ax.text(mid_x, mid_y, f"E{idx}", color="green", fontsize=10)

    # Set plot limits
    if bounding_square:
        ax.set_xlim(box_min[0], box_max[0])
        ax.set_ylim(box_min[1], box_max[1])
        square = Rectangle(
            box_min,
            length,
            length,
            edgecolor="black",
            facecolor="none",
            linestyle="--",
            linewidth=1.5,
        )
        ax.add_patch(square)
    else:
        all_x = np.concatenate((points[:, 0], vor.vertices[:, 0]))
        all_y = np.concatenate((points[:, 1], vor.vertices[:, 1]))
        padding_x = (all_x.max() - all_x.min()) * 0.1 or 1
        padding_y = (all_y.max() - all_y.min()) * 0.1 or 1
        ax.set_xlim(all_x.min() - padding_x, all_x.max() + padding_x)
        ax.set_ylim(all_y.min() - padding_y, all_y.max() + padding_y)

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title("Voronoi Diagram")
    plt.grid(True)
    plt.savefig(save_path, dpi=300)
    plt.show()
    plt.close(fig)


if __name__ == "__main__":
    # Define some example points in a grid
    points = np.array([[i, j] for i in range(-3, 4) for j in range(-3, 4)])

    # Call with optional bounding square centered at (0,0) with side length 8
    plot_voronoi(points, bounding_square=(0, 0, 8))
