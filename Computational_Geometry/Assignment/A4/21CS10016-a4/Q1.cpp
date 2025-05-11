/*
    * Author: Bratin Mondal
    * Roll No: 21CS10016

    * Department of Computer Science and Engineering
    * Indian Institute of Technology, Kharagpur
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cmath>
#include <string>
#include <random>

/**
 * @class Point
 * @brief Represents a 2D point with integer coordinates.
 */
class Point
{
public:
    int x, y;

    /**
     * @brief Default constructor initializing the point to the origin (0, 0).
     */
    Point() : x(0), y(0) {}

    /**
     * @brief Parameterized constructor initializing the point to (a, b).
     * @param a The x-coordinate.
     * @param b The y-coordinate.
     */
    Point(int a, int b) : x(a), y(b) {}

    /**
     * @brief Overloaded equality operator to compare two points.
     * @param other The other point to compare.
     * @return True if both x and y coordinates are equal.
     */
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }

    /**
     * @brief Overloaded inequality operator to compare two points.
     * @param other The other point to compare.
     * @return True if either x or y coordinates differ.
     */
    bool operator!=(const Point &other) const
    {
        return !(*this == other);
    }
};

/**
 * @class GeometryUtils
 * @brief Provides utility functions for common geometric operations.
 */
class GeometryUtils
{
public:
    /**
     * @brief Computes the greatest common divisor (GCD) of two integers.
     * @param a First integer.
     * @param b Second integer.
     * @return The greatest common divisor of a and b.
     */
    static int computeGcd(int a, int b)
    {
        a = std::abs(a);
        b = std::abs(b);
        while (b != 0)
        {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    /**
     * @brief Determines the orientation of three points (a, b, c).
     *
     * Uses the cross product to determine the orientation:
     * - Positive: Counterclockwise (left turn)
     * - Zero: Collinear
     * - Negative: Clockwise (right turn)
     *
     * @param a First point.
     * @param b Second point.
     * @param c Third point.
     * @return A positive, zero, or negative integer based on orientation.
     */
    static int orientation(const Point &a, const Point &b, const Point &c)
    {
        int cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        return cross;
    }

    /**
     * @brief Computes the signed area of a polygon.
     * @param poly Vector of points representing the polygon vertices.
     * @return The signed area (positive if vertices are in counterclockwise order).
     */
    static double polygonArea(const std::vector<Point> &poly)
    {
        double area = 0.0;
        int n = poly.size();
        for (int i = 0; i < n; ++i)
        {
            int j = (i + 1) % n;
            area += poly[i].x * poly[j].y - poly[j].x * poly[i].y;
        }
        return area / 2.0;
    }

    /**
     * @brief Computes the normalized slope between two points.
     *
     * For vertical or horizontal lines, special cases are handled.
     *
     * @param a First point.
     * @param b Second point.
     * @return A pair (dy, dx) representing the normalized slope.
     */
    static std::pair<int, int> getNormalizedSlope(const Point &a, const Point &b)
    {
        int dx = b.x - a.x;
        int dy = b.y - a.y;

        // Handle vertical line.
        if (dx == 0)
        {
            return std::make_pair(1, 0);
        }
        // Handle horizontal line.
        else if (dy == 0)
        {
            return std::make_pair(0, 1);
        }
        else
        {
            // Ensure dx is positive for consistency.
            if (dx < 0)
            {
                dx = -dx;
                dy = -dy;
            }
            int g = computeGcd(dx, dy);
            dx /= g;
            dy /= g;
            return std::make_pair(dy, dx);
        }
    }
};

/**
 * @class PointGenerator
 * @brief Generates a set of non-collinear points from a predefined grid.
 *
 * The grid is constructed with points spaced 10 units apart from (10,10) to (800,600).
 */
class PointGenerator
{
private:
    std::vector<Point> grid; // Stores all candidate grid points.

    /**
     * @brief Randomly shuffles the grid of candidate points.
     */
    void randomizeGrid()
    {
        std::shuffle(grid.begin(), grid.end(), std::default_random_engine(std::rand()));
    }

public:
    /**
     * @brief Constructor that initializes the grid of points.
     */
    PointGenerator()
    {
        grid.clear();
        // Create grid points with x in [10, 800] and y in [10, 600] stepping by 10.
        for (int x = 10; x <= 800; x += 10)
        {
            for (int y = 10; y <= 600; y += 10)
            {
                grid.push_back(Point(x, y));
            }
        }
        // Seed the random number generator with the current time.
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    /**
     * @brief Generates a specified number of non-collinear points.
     * @param numPoints The number of points to generate.
     * @return A vector of non-collinear points.
     */
    std::vector<Point> generateNonCollinearPoints(int numPoints)
    {
        std::vector<Point> points;

        // Continue until the required number of non-collinear points are selected.
        while (points.size() < static_cast<size_t>(numPoints))
        {
            randomizeGrid(); // Shuffle the grid to randomize the selection order.
            std::vector<bool> discarded(grid.size(), false);
            points.clear();

            for (size_t i = 0; i < grid.size(); i++)
            {
                if (discarded[i])
                    continue;

                points.push_back(grid[i]);
                if (points.size() == static_cast<size_t>(numPoints))
                    break;

                // Map to record slopes from the current point to all previously processed points.
                std::map<std::pair<int, int>, int> slopes;
                for (size_t j = 0; j < i; j++)
                {
                    if (discarded[j])
                        continue;
                    auto slope = GeometryUtils::getNormalizedSlope(grid[i], grid[j]);
                    slopes[slope]++;
                }
                // Mark all later points that share a slope with grid[i] as collinear.
                for (size_t j = i + 1; j < grid.size(); j++)
                {
                    if (discarded[j])
                        continue;
                    auto slope = GeometryUtils::getNormalizedSlope(grid[i], grid[j]);
                    if (slopes.find(slope) != slopes.end())
                    {
                        discarded[j] = true;
                    }
                }
            }
        }
        return points;
    }
};

/**
 * @class FileOutputHandler
 * @brief Handles writing polygon data to text and SVG files.
 */
class FileOutputHandler
{
public:
    /**
     * @brief Writes the polygon vertices to a text file.
     *
     * The file begins with the number of vertices, followed by each vertex’s coordinates.
     *
     * @param filename The name of the text file.
     * @param polygon The vector of polygon vertices.
     */
    static void writeTextFile(const std::string &filename, const std::vector<Point> &polygon)
    {
        std::ofstream ofs(filename.c_str());
        if (!ofs)
        {
            std::cerr << "Error opening " << filename << " for writing." << std::endl;
            exit(1);
        }
        ofs << polygon.size() << std::endl;
        for (size_t i = 0; i < polygon.size(); i++)
        {
            ofs << polygon[i].x << " " << polygon[i].y << std::endl;
        }
        ofs.close();
    }

    /**
     * @brief Writes the polygon to an SVG file with coordinate corrections.
     *
     * Since SVG's y-axis increases downward, we subtract each point's y value from the
     * SVG canvas height to flip the coordinate system, ensuring a conventional Cartesian
     * display.
     *
     * @param filename The name of the SVG file.
     * @param polygon The vector of polygon vertices.
     */
    static void writeSVGFile(const std::string &filename, const std::vector<Point> &polygon)
    {
        std::ofstream ofs(filename.c_str());
        if (!ofs)
        {
            std::cerr << "Error opening " << filename << " for writing." << std::endl;
            exit(1);
        }

        // Define SVG canvas dimensions.
        int svgWidth = 840;
        int svgHeight = 640;

        // Begin SVG document with defined dimensions and namespace.
        ofs << "<svg width=\"" << svgWidth << "px\" height=\"" << svgHeight << "px\" "
            << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << std::endl;

        // Start the polygon element and list vertices with corrected y coordinates.
        ofs << "<polygon points=\"";
        for (size_t i = 0; i < polygon.size(); i++)
        {
            // Correct the y coordinate: subtract point.y from svgHeight.
            int correctedY = svgHeight - polygon[i].y;
            ofs << polygon[i].x << "," << correctedY << " ";
        }
        ofs << "\" style=\"fill:none;stroke:black;stroke-width:2\" />" << std::endl;

        // Draw circles at each vertex with corrected y coordinates.
        for (size_t i = 0; i < polygon.size(); i++)
        {
            int correctedY = svgHeight - polygon[i].y;
            ofs << "<circle cx=\"" << polygon[i].x << "\" cy=\"" << correctedY
                << "\" r=\"3\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" />" << std::endl;
        }
        ofs << "</svg>" << std::endl;
        ofs.close();
    }
};

/**
 * @class XMonotonePolygon
 * @brief Generates an x-monotone polygon from a set of points.
 *
 * The polygon is constructed by partitioning the points into upper and lower chains
 * based on their x-coordinates and then concatenating them in order.
 */
class XMonotonePolygon
{
private:
    int numVertices;               ///< Number of vertices for the polygon.
    std::vector<Point> pts;        ///< Selected non-collinear points.
    std::vector<Point> polygon;    ///< Final polygon vertices in counterclockwise order.
    PointGenerator pointGenerator; ///< Utility to generate non-collinear grid points.

    /**
     * @brief Partitions points into upper (A) and lower (B) chains.
     *
     * The leftmost (sleft) and rightmost (sright) points are identified, and then the
     * remaining points are partitioned based on their orientation relative to the line
     * joining these extremes.
     *
     * @param A Vector to hold points in the upper chain.
     * @param B Vector to hold points in the lower chain.
     * @param sleft Reference to store the leftmost point.
     * @param sright Reference to store the rightmost point.
     */
    void partitionPoints(std::vector<Point> &A, std::vector<Point> &B, Point &sleft, Point &sright) const
    {
        if (pts.empty())
            return;

        sleft = pts[0];
        sright = pts[0];

        // Identify the leftmost and rightmost points.
        for (size_t i = 0; i < pts.size(); i++)
        {
            const Point &p = pts[i];
            if (p.x < sleft.x || (p.x == sleft.x && p.y < sleft.y))
                sleft = p;
            if (p.x > sright.x || (p.x == sright.x && p.y < sright.y))
                sright = p;
        }

        // Partition remaining points based on their orientation with respect to sleft and sright.
        for (size_t i = 0; i < pts.size(); i++)
        {
            const Point &p = pts[i];
            if ((p.x == sleft.x && p.y == sleft.y) || (p.x == sright.x && p.y == sright.y))
                continue;

            int cp = GeometryUtils::orientation(sleft, sright, p);
            if (cp >= 0)
                A.push_back(p); // Point lies on or above the line (upper chain).
            else
                B.push_back(p); // Point lies below the line (lower chain).
        }
    }

    /**
     * @brief Constructs the x-monotone polygon.
     *
     * Combines the leftmost point, sorted upper chain, rightmost point, and sorted lower chain
     * to form the final polygon. If the vertices are not in counterclockwise order, the order is reversed.
     */
    void constructPolygon()
    {
        Point sleft, sright;
        std::vector<Point> A, B;
        partitionPoints(A, B, sleft, sright);

        // Sort upper chain in increasing x order.
        std::sort(A.begin(), A.end(), [](const Point &a, const Point &b)
                  { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); });
        // Sort lower chain in decreasing x order.
        std::sort(B.begin(), B.end(), [](const Point &a, const Point &b)
                  { return (a.x != b.x) ? (a.x > b.x) : (a.y > b.y); });

        polygon.clear();
        polygon.push_back(sleft);
        for (size_t i = 0; i < A.size(); i++)
        {
            polygon.push_back(A[i]);
        }
        polygon.push_back(sright);
        for (size_t i = 0; i < B.size(); i++)
        {
            polygon.push_back(B[i]);
        }

        // Ensure the polygon's vertices are in counterclockwise order.
        if (GeometryUtils::polygonArea(polygon) < 0)
            std::reverse(polygon.begin(), polygon.end());
    }

public:
    /**
     * @brief Constructor for XMonotonePolygon.
     * @param n The number of vertices (must be between 3 and 100).
     */
    XMonotonePolygon(int n) : numVertices(n)
    {
        if (n < 3 || n > 100)
        {
            std::cerr << "Invalid input! n must be between 3 and 100." << std::endl;
            exit(1);
        }
    }

    /**
     * @brief Generates the x-monotone polygon.
     *
     * Non-collinear points are generated, and the polygon is constructed from them.
     */
    void generatePolygon()
    {
        pts = pointGenerator.generateNonCollinearPoints(numVertices);
        constructPolygon();
    }

    /**
     * @brief Outputs the polygon data to text and SVG files.
     *
     * @param txtFileName The name of the output text file (default "monopoly.txt").
     * @param svgFileName The name of the output SVG file (default "monopoly.svg").
     */
    void outputFiles(const std::string &txtFileName = "monopoly.txt",
                     const std::string &svgFileName = "monopoly.svg") const
    {
        FileOutputHandler::writeTextFile(txtFileName, polygon);
        FileOutputHandler::writeSVGFile(svgFileName, polygon);
    }
};

/**
 * @brief Main function.
 *
 * Reads the number of vertices from the command line, generates the x-monotone polygon,
 * and outputs the polygon data to text and SVG files.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Exit status.
 */
int main(int argc, char *argv[])
{
    // Validate command-line arguments.
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number of vertices (3 to 100)>" << std::endl;
        return 1;
    }

    int n;
    try
    {
        n = std::stoi(argv[1]);
    }
    catch (...)
    {
        std::cerr << "Invalid input! Please enter an integer." << std::endl;
        return 1;
    }

    if (n < 3 || n > 100)
    {
        std::cerr << "Invalid input! n must be between 3 and 100." << std::endl;
        return 1;
    }

    // Create, generate, and output the x-monotone polygon.
    XMonotonePolygon poly(n);
    poly.generatePolygon();
    poly.outputFiles();

    std::cout << "Files 'monopoly.txt' and 'monopoly.svg' generated successfully." << std::endl;
    return 0;
}
