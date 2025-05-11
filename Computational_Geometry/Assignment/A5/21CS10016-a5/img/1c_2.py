"""
Author: Bratin Mondal
Roll No: 21CS10016
Deparment of Computer Science and Engineering
Indian Institue of Technology, Kharagpur
"""

import numpy as np
import matplotlib.pyplot as plt

# -----------------------------------------------------
# 1) Define the sites and directrix
#    - "site 0" is the single top site.
#    - 10 collinear sites on y=0, spaced along x.
#    - Directrix slightly below y=0.
# -----------------------------------------------------
site0 = (0, 7)   # Previously "top site"
# 10 equally spaced bottom sites from x=-7 to x=7 in steps of 2
site_bottoms = [(x, 0) for x in range(-7, 8, 2)]
directrix_y = -0.05

# Combine all sites into a list, tagging each site with a color.
# site0 => gray, bottom sites => purple.
all_sites = [(site0, 'gray')] + [(sb, 'purple') for sb in site_bottoms]

# -----------------------------------------------------
# 2) Parabola function for site & directrix
# -----------------------------------------------------
def parabola_y(site, directrix, x):
    xs, ys = site
    d = directrix
    # (x - xs)^2 + (y - ys)^2 = (y - d)^2  => solve for y
    numerator   = (x - xs)**2 + (ys**2 - d**2)
    denominator = 2.0 * (ys - d)
    return numerator / denominator

# -----------------------------------------------------
# 3) Generate a wide range of x-values to capture all sites
# -----------------------------------------------------
x_vals = np.linspace(-10, 10, 1000)

# Compute y-values for each parabola
parabolas = []
for (site, color) in all_sites:
    y_vals = parabola_y(site, directrix_y, x_vals)
    parabolas.append((y_vals, color))

# -----------------------------------------------------
# 4) Plot all parabolas thinly for reference
# -----------------------------------------------------
plt.figure(figsize=(10,6))
for (y_vals, color) in parabolas:
    plt.plot(x_vals, y_vals, color=color, linewidth=1)

# -----------------------------------------------------
# 5) Determine the beach line (lower envelope)
# -----------------------------------------------------
min_index = np.zeros_like(x_vals, dtype=int)
min_values = np.full_like(x_vals, np.inf)

for i in range(len(x_vals)):
    for p_idx, (y_vals, _) in enumerate(parabolas):
        if y_vals[i] < min_values[i]:
            min_values[i] = y_vals[i]
            min_index[i] = p_idx

# Plot each maximal interval where the same parabola is minimal
start = 0
for i in range(1, len(x_vals)):
    if min_index[i] != min_index[i-1]:
        idx = min_index[start]
        (y_vals, color) = parabolas[idx]
        plt.plot(x_vals[start:i], y_vals[start:i], color=color, linewidth=3)
        start = i

# Final segment
idx = min_index[start]
(y_vals, color) = parabolas[idx]
plt.plot(x_vals[start:], y_vals[start:], color=color, linewidth=3)

# -----------------------------------------------------
# 6) Plot the sites and the sweep line
# -----------------------------------------------------
# (a) Sweep line (directrix) in gold, labeled "sweep line"
plt.axhline(y=directrix_y, color='gold', linewidth=3)
plt.text(11.1, directrix_y, "sweep\nline", va='center', ha='left', fontsize=12, color='black')

# (b) Mark site 0 (previously top site) as a black dot
plt.plot(site0[0], site0[1], 'ko', markersize=8)
plt.text(site0[0] + 0.1, site0[1] + 0.1, "site 0", color='black')

# (c) Bottom sites (black dots + labels)
for i, sb in enumerate(site_bottoms, start=1):
    plt.plot(sb[0], sb[1], 'ko', markersize=8)
    plt.text(sb[0] + 0.1, sb[1] + 0.1, f"Site {i}", color='black')

# -----------------------------------------------------
# 7) Final adjustments
# -----------------------------------------------------
plt.gca().set_aspect('equal', 'datalim')
# Show more space in x and y
plt.xlim(-11, 11)
plt.ylim(directrix_y - 1, 6)

plt.xlabel("x")
plt.ylabel("y")
plt.title("A Single 'site 0' and 8 Collinear Bottom Sites\n"
          "'site 0' contributes to the beach line linear number of times")


plt.savefig("1c_2.png", bbox_inches='tight')

plt.show()
