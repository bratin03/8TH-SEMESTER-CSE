"""
Author: Bratin Mondal
Roll No: 21CS10016
Department of Computer Science and Engineering
Indian Institute of Technology, Kharagpur
"""

import numpy as np
import matplotlib.pyplot as plt
import itertools

def circle_from_3pts(p1, p2, p3):
    """
    Compute the circumcenter and circumradius of a circle passing through three non-collinear points.
    Returns (center, radius). If points are collinear, returns (None, None).
    """
    A = np.array([[p2[0]-p1[0], p2[1]-p1[1]],
                  [p3[0]-p1[0], p3[1]-p1[1]]])
    B = np.array([((p2[0]**2 - p1[0]**2)+(p2[1]**2 - p1[1]**2))/2,
                  ((p3[0]**2 - p1[0]**2)+(p3[1]**2 - p1[1]**2))/2])
    try:
        center = np.linalg.solve(A, B)
    except np.linalg.LinAlgError:
        return None, None
    radius = np.linalg.norm(np.array(p1) - center)
    return center, radius

def validate_general_position(points, tol=1e-9):
    """
    Validate that points are in general position.
    - No three points are collinear.
    - No four points lie on the same circle.
    Returns True if the condition holds, otherwise False.
    """
    # Check that no three points are collinear.
    for combo in itertools.combinations(points, 3):
        p1, p2, p3 = combo
        # Compute the area of the triangle using the determinant formula
        area = 0.5 * abs(p1[0]*(p2[1]-p3[1]) + p2[0]*(p3[1]-p1[1]) + p3[0]*(p1[1]-p2[1]))
        if area < tol:
            print("Collinear points found:", p1, p2, p3)
            return False

    # Check that no four points are concyclic.
    for quad in itertools.combinations(points, 4):
        # Test every combination of three out of the four points.
        for combo3 in itertools.combinations(quad, 3):
            p_a, p_b, p_c = combo3
            center, radius = circle_from_3pts(p_a, p_b, p_c)
            if center is None:
                continue  # points were collinear; already caught above
            # Find the remaining point in the quadruple.
            remaining = [pt for pt in quad if not any(np.array_equal(pt, x) for x in combo3)][0]
            if abs(np.linalg.norm(remaining - center) - radius) < tol:
                print("Concyclic points found:", combo3, remaining)
                return False
    return True

def print_circumcenters(points, tol=1e-9):
    """
    For every combination of three sites, compute and print the circumcenter and circumradius.
    """
    # Get the minimum y coordinate for the points
    min_y = np.min(points[:, 1])
    print("\nCircumcenters for all combinations of three sites:")
    for combo in itertools.combinations(points, 3):
        p1, p2, p3 = combo
        center, radius = circle_from_3pts(p1, p2, p3)
        # Check for collinearity
        if center is None:
            print(f"For points {p1}, {p2}, {p3}: The points are collinear; no unique circumcenter.")
        else:
            print(f"For points {p1}, {p2}, {p3}: Circumcenter = {center}, Radius = {radius}")
            # Check if the circumcenter is above the minimum y coordinate (print in red)
            if center[1] > min_y + tol:
                print(f"  Circumcenter {center} is above the minimum y coordinate {min_y}.")
                exit(1)
        

# Define the six sites
points = np.array([
    [2, 5],
    [-2, 5.01],
    [4, 4.98],
    [-4, 4.99],
    [6, 4.91],
    [-6, 4.91],
])

# Validate general position
if validate_general_position(points):
    print("Points are in general position.")
else:
    print("Points are not in general position.")
    exit(1)

# Print the circumcenters for every combination of three points.
print_circumcenters(points)

# ----------------------------
# Plot: Original Input Points
# ----------------------------
plt.figure(figsize=(8, 6))
for idx, point in enumerate(points):
    plt.plot(point[0], point[1], "bo")  # blue circle for each site
    plt.text(point[0] + 0.1, point[1] + 0.1, f"Site {idx+1}", color="blue")

plt.title("Original Input Sites")
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.grid(True)

# Set limits with a small margin around the points.
margin = 1
x_min = np.min(points[:, 0]) - margin
x_max = np.max(points[:, 0]) + margin
y_min = np.min(points[:, 1]) - margin
y_max = np.max(points[:, 1]) + margin
plt.xlim(x_min, x_max)
plt.ylim(y_min, y_max)

plt.savefig("1b.png", dpi=300)
