import random
import json
from operator import itemgetter
import numpy
import matplotlib.pyplot as plt

class PolygonAPI:
    def __init__(self, min_rand_coord=None, max_rand_coord=None, points_num=None, predefined_polygon=None, config_file=None):
        """
        Initializes the Polygon API with parameters to create a polygon.
        
        :param min_rand_coord: Minimum random coordinate (int), defaults to None for random range
        :param max_rand_coord: Maximum random coordinate (int), defaults to None for random range
        :param points_num: Number of points in the polygon, defaults to None for random points
        :param predefined_polygon: A predefined list of tuples representing polygon points, defaults to None
        :param config_file: Path to a JSON config file that contains predefined points, defaults to None
        """
        self.array = predefined_polygon
        self.min_rand_coord = min_rand_coord
        self.max_rand_coord = max_rand_coord
        self.points_num = points_num
        self.config_file = config_file
        self.save_path = None

        if self.config_file:
            self.load_points_from_config()

    def load_points_from_config(self):
        """Load polygon points and settings from a given config.json file."""
        try:
            with open(self.config_file, 'r') as f:
                config_data = json.load(f)
                self.array = config_data.get("polygon_points", [])
                self.save_path = config_data.get("save_path", "polygon_plot.png")
                if not self.array and not self.save_path:
                    raise ValueError("Config file must contain either points or a save path.")
        except Exception as e:
            print(f"Error loading config file: {e}")

    def generate_random_points(self):
        """Generates random points within given range."""
        random_coords_list = []
        for _ in range(self.points_num):
            coords_tuple = (random.randint(self.min_rand_coord, self.max_rand_coord),
                            random.randint(self.min_rand_coord, self.max_rand_coord))
            random_coords_list.append(coords_tuple)
        self.array = random_coords_list
        return random_coords_list
    
    def close_line_to_polygon(self):
        """Closes the polygon by adding the first point at the end if necessary."""
        if self.array[0] != self.array[-1]:
            self.array.append(self.array[0])

    def find_leftmost_point(self):
        """Finds the leftmost point in the polygon. In case of a tie, the point with the lowest y-coordinate is returned."""
        return min(self.array, key=lambda point: (point[0], -point[1]))  # Prioritize smallest x, then smallest y

    def find_rightmost_point(self):
        """Finds the rightmost point in the polygon. In case of a tie, the point with the lowest y-coordinate is returned."""
        return max(self.array, key=lambda point: (point[0], -point[1]))  # Prioritize largest x, then smallest y

    def is_point_above_the_line(self, point, line_points):
        """Determines if a point is above, below, or on a line."""
        px, py = point
        P1, P2 = line_points
        P1x, P1y = P1
        P2x, P2y = P2
        array = numpy.array([[P1x - px, P1y - py], [P2x - px, P2y - py]])
        det = numpy.linalg.det(array)
        return numpy.sign(det)
    
    def sort_array_into_A_B_C(self, line_points):
        """Sorts the points of the polygon into three categories based on their position relative to the line."""
        A_array, B_array, C_array = [], [], []
        for point in self.array:
            sign = self.is_point_above_the_line(point, line_points)
            if sign == 0:
                C_array.append(point)
            elif sign == -1:
                A_array.append(point)
            elif sign == 1:
                B_array.append(point)
        return A_array, B_array, C_array

    def sort_and_merge_A_B_C_arrays(self, A_array, B_array, C_array):
        """Sorts and merges the A, B, and C arrays to form the final polygon order."""
        A_C_array = sorted(A_array + C_array, key=itemgetter(0))
        B_array.sort(key=itemgetter(0), reverse=True)
        self.array = A_C_array + B_array

    def save_image(self, A_array, B_array, C_array, line_points):
        """Saves a plot of the polygon and the sorted arrays to a file."""
        [(x_lm, y_lm), (x_rm, y_rm)] = line_points
        x = [point[0] for point in self.array]
        y = [point[1] for point in self.array]
        Ax = [point[0] for point in A_array]
        Ay = [point[1] for point in A_array]
        Bx = [point[0] for point in B_array]
        By = [point[1] for point in B_array]
        Cx = [point[0] for point in C_array]
        Cy = [point[1] for point in C_array]
        
        plt.plot(Ax, Ay, 'o', c='orange')  # below the line
        plt.plot(Bx, By, 'o', c='blue')    # above the line
        plt.plot(Cx, Cy, 'o', c='black')   # on the line
        plt.plot(x_lm, y_lm, 'o', c='green')  # leftmost point
        plt.plot(x_rm, y_rm, 'o', c='red')   # rightmost point
        plt.plot([x_lm, x_rm], [y_lm, y_rm], linestyle=':', color='black', linewidth=0.5)  # polygon's division line
        plt.plot(x, y, color='black', linewidth=1)  # connect points by line
        plt.savefig(self.save_path)
        print(f"Plot saved to {self.save_path}")
        plt.close()

    def create_polygon(self):
        """Main method to create the polygon."""
        if self.array is None:
            if None in [self.min_rand_coord, self.max_rand_coord, self.points_num]:
                raise ValueError("Random polygon parameters are not properly set.")
            self.array = self.generate_random_points()

        leftmost_point = self.find_leftmost_point()
        rightmost_point = self.find_rightmost_point()
        line_points = [leftmost_point, rightmost_point]

        A_array, B_array, C_array = self.sort_array_into_A_B_C(line_points)
        self.sort_and_merge_A_B_C_arrays(A_array, B_array, C_array)
        self.close_line_to_polygon()

        self.save_image(A_array, B_array, C_array, line_points)

        return self.array

# Example usage of the PolygonAPI class:
def main():
    config_file = 'config.json'  # Path to your config file with predefined points
    min_rand_coord = 1
    max_rand_coord = 100
    points_num = 30
    
    # Create an instance of the PolygonAPI class
    polygon_api = PolygonAPI(min_rand_coord=min_rand_coord, max_rand_coord=max_rand_coord, points_num=points_num, config_file=config_file)

    # Generate the polygon
    polygon_array = polygon_api.create_polygon()
    print("Generated Polygon Coordinates:", polygon_array)

if __name__ == "__main__":
    main()
