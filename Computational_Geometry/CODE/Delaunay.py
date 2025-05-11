import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import Delaunay

def plot_delaunay(points, show=True, annotate=False):
    """
    Given an (N,2) array of 2D points, compute and plot their Delaunay triangulation.

    Parameters
    ----------
    points : array-like, shape (n_points, 2)
        The input 2D points.
    show : bool
        If True, calls plt.show() at the end.
    annotate : bool
        If True, labels each point by its index.
    """
    # Ensure numpy array
    pts = np.asarray(points)
    if pts.ndim != 2 or pts.shape[1] != 2:
        raise ValueError("points must be of shape (n_points, 2)")

    # Compute Delaunay triangulation
    tri = Delaunay(pts)  # :contentReference[oaicite:2]{index=2}

    # Plot the triangulation edges
    plt.triplot(pts[:, 0], pts[:, 1], tri.simplices, linewidth=1.0)  # :contentReference[oaicite:3]{index=3}

    # Plot the points
    plt.plot(pts[:, 0], pts[:, 1], 'o', markersize=5)

    # Optionally annotate point indices
    if annotate:
        for i, (x, y) in enumerate(pts):
            plt.text(x, y, str(i), color='red', fontsize=12)

    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Delaunay Triangulation')
    plt.gca().set_aspect('equal')

    if show:
        plt.show()

if __name__ == "__main__":
    # Example usage: random points
    pts = np.array([[0, 0], [1, 0], [2, 1], [0, 2]])
    plot_delaunay(pts, annotate=True)
