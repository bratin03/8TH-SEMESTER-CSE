"""
Author: Bratin Mondal
Roll No: 21CS10016
Deparment of Computer Science and Engineering
Indian Institue of Technology, Kharagpur
"""

import numpy as np
import matplotlib.pyplot as plt

# -----------------------------------------------------
# 1) Define two sites and the sweep line (directrix)
# -----------------------------------------------------
site1 = (0, 2)  # Site 1 (will generate a parabola)
site2 = (0, 1)  # Site 2 (will generate a parabola)
sweep_line = -1.0


# -----------------------------------------------------
# 2) Parabola function from site & sweep line
#    For a site = (xs, ys) and sweep line y = d,
#    the parabola is all points (x,y) where
#    distance to site == distance to sweep line.
#
#    sqrt((x - xs)^2 + (y - ys)^2) = y - d   (assuming y > d)
#
#    Solve for y in terms of x:
#    (x - xs)^2 + (y - ys)^2 = (y - d)^2
# -----------------------------------------------------
def parabola_y(site, directrix, x):
    xs, ys = site
    d = directrix
    numerator = (x - xs) ** 2 + (ys**2 - d**2)
    denominator = 2.0 * (ys - d)
    return numerator / denominator


# -----------------------------------------------------
# 3) Sample a range of x-values and compute each parabola
# -----------------------------------------------------
x_vals = np.linspace(-5, 5, 500)
y_site1 = parabola_y(site1, sweep_line, x_vals)
y_site2 = parabola_y(site2, sweep_line, x_vals)

# -----------------------------------------------------
# 4) Plot the entire parabolas as thin lines (no labels)
# -----------------------------------------------------
plt.figure(figsize=(7, 7))
plt.plot(x_vals, y_site1, color="gray", linewidth=1)
plt.plot(x_vals, y_site2, color="purple", linewidth=1)

# -----------------------------------------------------
# 5) Plot the sweep line and label it
# -----------------------------------------------------
plt.axhline(y=sweep_line, color="gold", linewidth=2)
plt.text(
    5.1, sweep_line, "sweep\nline", va="center", ha="left", fontsize=12, color="black"
)

# -----------------------------------------------------
# 6) Plot the sites as black dots and annotate them as "site 1" and "site 2"
# -----------------------------------------------------
plt.plot(site1[0], site1[1], "ko", markersize=8)
plt.text(site1[0] + 0.1, site1[1] + 0.1, "site 1", fontsize=12, color="black")
plt.plot(site2[0], site2[1], "ko", markersize=8)
plt.text(site2[0] + 0.1, site2[1] + 0.1, "site 2", fontsize=12, color="black")

# -----------------------------------------------------
# 7) Find intersection points to split the x-axis into intervals
#    We'll check where the difference between the two parabola
#    values changes sign.
# -----------------------------------------------------
diff = y_site1 - y_site2
x_intersections = []
for i in range(len(x_vals) - 1):
    if diff[i] * diff[i + 1] < 0:
        x0, x1 = x_vals[i], x_vals[i + 1]
        y0, y1 = diff[i], diff[i + 1]
        x_int = x0 - y0 * (x1 - x0) / (y1 - y0)
        x_intersections.append(x_int)

# Define breakpoints from left edge, intersection(s), to right edge
x_breaks = [x_vals[0]] + x_intersections + [x_vals[-1]]
x_breaks.sort()

# -----------------------------------------------------
# 8) On each interval, determine which parabola is lower
#    and overlay that segment in a bold line.
# -----------------------------------------------------
for i in range(len(x_breaks) - 1):
    x_start, x_end = x_breaks[i], x_breaks[i + 1]
    mask = (x_vals >= x_start) & (x_vals <= x_end)
    x_seg = x_vals[mask]
    y_seg_site1 = y_site1[mask]
    y_seg_site2 = y_site2[mask]

    # Evaluate at the midpoint to decide which one is lower
    x_mid = 0.5 * (x_start + x_end)
    y1_mid = parabola_y(site1, sweep_line, x_mid)
    y2_mid = parabola_y(site2, sweep_line, x_mid)

    if y1_mid < y2_mid:
        plt.plot(x_seg, y_seg_site1, color="gray", linewidth=3)
    else:
        plt.plot(x_seg, y_seg_site2, color="purple", linewidth=3)

# -----------------------------------------------------
# 9) Final cosmetic adjustments
# -----------------------------------------------------
plt.gca().set_aspect("equal", "datalim")
plt.xlim(-5, 5)
plt.ylim(sweep_line - 0.5, 3)
plt.xlabel("x")
plt.ylabel("y")
plt.title("'site 1' and 'site 2' with sweep line\n"
          "'site 1' contributes to the beach line twice")

plt.savefig("1c_1.png", dpi=300)
plt.show()
