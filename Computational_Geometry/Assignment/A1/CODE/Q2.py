import matplotlib.pyplot as plt

class ConvexPolygonAPI:
    def __init__(self, points):
        """
        Initializes the ConvexPolygonAPI with predefined points in anticlockwise order.
        
        :param points: A list of tuples representing the points of the convex polygon.
        """
        self.points = points

    def draw_polygon(self):
        """
        Draws the convex polygon and labels the vertices.
        """
        if not self.points:
            raise ValueError("No points provided for the polygon.")
        
        # Unzip the points into x and y coordinates
        x, y = zip(*self.points)

        # Add the first point at the end to close the polygon
        x += (x[0],)
        y += (y[0],)

        # Plot the polygon
        plt.plot(x, y, marker='o', color='black', linestyle='-', linewidth=1)

        # Label each vertex as P_1, P_2, ...
        for i, (xi, yi) in enumerate(self.points):
            plt.text(xi, yi, f'P_{i+1}', fontsize=12, ha='right' if xi < 0 else 'left', va='bottom' if yi < 0 else 'top')

        # Set axis to be equal for better aspect ratio
        plt.gca().set_aspect('equal', adjustable='box')

        # Remove grid and axis
        plt.axis('off')  # Turn off the axis
        plt.gca().set_xticks([])  # Remove x-ticks
        plt.gca().set_yticks([])  # Remove y-ticks

        # Display the plot
        plt.title('Convex Polygon with Vertices')
        plt.show()

# Example usage of the ConvexPolygonAPI class
def main():
    # Define a convex polygon with points in anticlockwise order
    points = [(2, 1), (3, 4), (5, 3), (6, 1), (4, -2)]
    
    # Create an instance of the ConvexPolygonAPI class
    convex_polygon_api = ConvexPolygonAPI(points)

    # Draw the polygon
    convex_polygon_api.draw_polygon()

if __name__ == "__main__":
    main()
