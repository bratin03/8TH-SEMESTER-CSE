/*
    * Author: Bratin Mondal
    * Roll No: 21CS10016

    * Department of Computer Science and Engineering
    * Indian Institute of Technology, Kharagpur
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using namespace std;

/**
 * @struct Vertex
 * @brief Represents a vertex of the polygon.
 */
struct Vertex
{
    int x, y;  /**< Coordinates of the vertex */
    int idx;   /**< Original index (assumed to be in counterclockwise order) */
    int chain; /**< Chain identifier (0 or 1) based on vertex location */
};

/**
 * @struct Diagonal
 * @brief Represents a diagonal in the polygon.
 */
struct Diagonal
{
    Vertex u, v; /**< Endpoints of the diagonal */
    string type; /**< Diagonal type: "D1" if endpoints lie on the same chain, "D2" otherwise */
};

/**
 * @class Polygon
 * @brief Encapsulates the polygon and its associated operations.
 */
class Polygon
{
private:
    vector<Vertex> vertices; /**< List of polygon vertices */

public:
    /**
     * @brief Loads the polygon from a file.
     *
     * The file should start with an integer n (number of vertices)
     * followed by n pairs of x and y coordinates.
     *
     * @param filename Name of the file containing the polygon data.
     * @return true if the file is loaded successfully; false otherwise.
     */
    bool loadFromFile(const string &filename)
    {
        ifstream infile(filename.c_str());
        if (!infile)
        {
            cerr << "Error opening file " << filename << endl;
            return false;
        }
        int n;
        infile >> n;
        if (n < 3)
        {
            cerr << "Polygon must have at least 3 vertices." << endl;
            return false;
        }
        vertices.resize(n);
        // Read each vertex from the file.
        for (int i = 0; i < n; i++)
        {
            infile >> vertices[i].x >> vertices[i].y;
            vertices[i].idx = i;    // Store the original index.
            vertices[i].chain = -1; // Chain will be assigned later.
        }
        infile.close();
        return true;
    }

    /**
     * @brief Assigns chains to vertices based on x-monotonicity.
     *
     * Finds the leftmost and rightmost vertices and assigns:
     *   - Chain 0: vertices from the leftmost to the rightmost (in polygon order)
     *   - Chain 1: the remaining vertices.
     */
    void assignChains()
    {
        int n = vertices.size();
        int leftmost = 0, rightmost = 0;
        // Identify leftmost and rightmost vertices.
        for (int i = 1; i < n; i++)
        {
            if (vertices[i].x < vertices[leftmost].x ||
                (vertices[i].x == vertices[leftmost].x && vertices[i].y < vertices[leftmost].y))
                leftmost = i;
            if (vertices[i].x > vertices[rightmost].x ||
                (vertices[i].x == vertices[rightmost].x && vertices[i].y < vertices[rightmost].y))
                rightmost = i;
        }
        // Assign chain 0 to vertices from leftmost to rightmost in order.
        int i = leftmost;
        vertices[i].chain = 0;
        while (i != rightmost)
        {
            i = (i + 1) % n;
            vertices[i].chain = 0;
        }
        // Assign chain 1 to the remaining vertices.
        for (int j = 0; j < n; j++)
        {
            if (vertices[j].chain == -1)
                vertices[j].chain = 1;
        }
    }

    /**
     * @brief Retrieves a sorted copy of the vertices.
     *
     * The vertices are sorted in increasing order of x (and y if tied)
     * using a two-pointer merge technique over the two chains.
     *
     * @return A vector of sorted vertices.
     */
    vector<Vertex> getSortedVertices() const
    {
        int n = vertices.size();
        int leftmost = 0, rightmost = 0;
        // Identify leftmost and rightmost vertices.
        for (int i = 1; i < n; i++)
        {
            if (vertices[i].x < vertices[leftmost].x ||
                (vertices[i].x == vertices[leftmost].x && vertices[i].y < vertices[leftmost].y))
                leftmost = i;
            if (vertices[i].x > vertices[rightmost].x ||
                (vertices[i].x == vertices[rightmost].x && vertices[i].y < vertices[rightmost].y))
                rightmost = i;
        }
        // Build chain0: vertices from leftmost to rightmost in polygon order.
        vector<Vertex> chain0;
        int i = leftmost;
        while (true)
        {
            chain0.push_back(vertices[i]);
            if (i == rightmost)
                break;
            i = (i + 1) % n;
        }
        // Build chain1: remaining vertices from rightmost back to leftmost.
        vector<Vertex> chain1;
        i = rightmost;
        while (true)
        {
            i = (i + 1) % n;
            if (i == leftmost)
                break;
            chain1.push_back(vertices[i]);
        }
        // Reverse chain1 so that its vertices are in increasing x order.
        reverse(chain1.begin(), chain1.end());

        // Merge the two chains using a two-pointer technique.
        vector<Vertex> merged;
        size_t p = 0, q = 0;
        while (p < chain0.size() && q < chain1.size())
        {
            if (chain0[p].x < chain1[q].x ||
                (chain0[p].x == chain1[q].x && chain0[p].y < chain1[q].y))
            {
                merged.push_back(chain0[p]);
                p++;
            }
            else
            {
                merged.push_back(chain1[q]);
                q++;
            }
        }
        // Append any remaining vertices from chain0.
        while (p < chain0.size())
        {
            merged.push_back(chain0[p]);
            p++;
        }
        // Append any remaining vertices from chain1.
        while (q < chain1.size())
        {
            merged.push_back(chain1[q]);
            q++;
        }
        return merged;
    }

    /**
     * @brief Returns the original list of vertices.
     *
     * @return Constant reference to the vector of vertices.
     */
    const vector<Vertex> &getVertices() const
    {
        return vertices;
    }

    /**
     * @brief Gets the number of vertices in the polygon.
     *
     * @return The vertex count.
     */
    int size() const
    {
        return vertices.size();
    }
};

/**
 * @class Triangulator
 * @brief Implements the stack-based triangulation algorithm.
 */
class Triangulator
{
private:
    const Polygon &poly; /**< Reference to the polygon to triangulate */

public:
    /**
     * @brief Constructor.
     *
     * @param polygon Reference to the polygon to be triangulated.
     */
    Triangulator(const Polygon &polygon) : poly(polygon) {}

    /**
     * @brief Computes the orientation of three vertices.
     *
     * Returns a positive value for a left turn, a negative value for a right turn,
     * and zero if the points are collinear.
     *
     * @param a First vertex.
     * @param b Second vertex.
     * @param c Third vertex.
     * @return An integer representing the orientation.
     */
    static int orientation(const Vertex &a, const Vertex &b, const Vertex &c)
    {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    /**
     * @brief Checks if two vertices are adjacent in the polygon.
     *
     * Two vertices are considered adjacent if their indices differ by 1 or if
     * they are the first and last vertices.
     *
     * @param a First vertex.
     * @param b Second vertex.
     * @param n Total number of vertices.
     * @return true if adjacent; false otherwise.
     */
    static bool isEdge(const Vertex &a, const Vertex &b, int n)
    {
        int diff = abs(a.idx - b.idx);
        return (diff == 1 || diff == n - 1);
    }

    /**
     * @brief Performs the triangulation of the polygon.
     *
     * Uses a stack-based approach to compute all the internal diagonals needed
     * for triangulation.
     *
     * @return A vector of Diagonals representing the triangulation.
     */
    vector<Diagonal> triangulate()
    {
        vector<Diagonal> diagonals;                           // Store computed diagonals.
        vector<Vertex> sortedPoly = poly.getSortedVertices(); // Get vertices sorted by x.
        int n = poly.size();

        vector<Vertex> S; // Stack for maintaining vertices during triangulation. (We use a vector for stack operations.)
        S.push_back(sortedPoly[0]);
        S.push_back(sortedPoly[1]);

        // Process each remaining vertex.
        for (int i = 2; i < n; i++)
        {
            Vertex curr = sortedPoly[i];
            // Case 1: Current vertex and stack top belong to different chains.
            if (curr.chain != S.back().chain)
            {
                // Connect current vertex with all vertices in the stack.
                while (S.size() > 1)
                {
                    Vertex top = S.back();
                    S.pop_back();
                    if (!isEdge(curr, top, n))
                    { // Ensure not an edge.
                        Diagonal diag;
                        diag.u = curr;
                        diag.v = top;
                        diag.type = "D2";
                        diagonals.push_back(diag);
                    }
                }
                // Reset the stack with the last processed vertex and the current one.
                S.clear();
                S.push_back(sortedPoly[i - 1]);
                S.push_back(curr);
            }
            // Case 2: Current vertex is on the same chain as the stack top.
            else
            {
                Vertex last = S.back();
                S.pop_back();
                // Check if a diagonal can be drawn with earlier vertices in the stack.
                while (!S.empty())
                {
                    Vertex second = S.back();
                    int orient = orientation(curr, last, second);
                    // For chain 0, interior is to the left; for chain 1, interior is to the right.
                    if ((curr.chain == 0 && orient < 0) || (curr.chain == 1 && orient > 0))
                    {
                        if (!isEdge(curr, second, n))
                        { // Only add if not adjacent.
                            Diagonal diag;
                            diag.u = curr;
                            diag.v = second;
                            diag.type = (curr.chain == second.chain) ? "D1" : "D2"; // D1 if same chain, D2 otherwise.
                            diagonals.push_back(diag);
                        }
                        last = second;
                        S.pop_back();
                    }
                    else
                    {
                        break;
                    }
                }
                S.push_back(last);
                S.push_back(curr);
            }
        }

        // Final step: Connect the rightmost vertex with all remaining vertices in the stack.
        Vertex lastVertex = sortedPoly[n - 1];
        S.pop_back(); // Remove the last vertex (itself) from the stack.
        while (S.size() > 1)
        {
            S.pop_back();
            if (!isEdge(lastVertex, S.back(), n))
            {
                Diagonal diag;
                diag.u = lastVertex;
                diag.v = S.back();
                diag.type = (lastVertex.chain == S.back().chain) ? "D1" : "D2"; // D1 if same chain, D2 otherwise.
                diagonals.push_back(diag);
            }
        }

        // Check if the triangulation is complete.
        if ((long long)diagonals.size() != (n - 3))
        {
            // The number of diagonals in a triangulated polygon with n vertices is n-3. If not, there is an error.
            cout << "Error in triangulation" << endl;
            exit(1);
        }

        return diagonals;
    }
};

/**
 * @class SVGDrawer
 * @brief Responsible for outputting the polygon and its diagonals as an SVG file.
 */
class SVGDrawer
{
public:
    /**
     * @brief Draws the polygon and diagonals to an SVG file.
     *
     * Corrects the y-coordinate for the SVG coordinate system (where y increases downward)
     * using a fixed canvas height of 640.
     *
     * @param polygon The polygon to be drawn.
     * @param diagonals The computed diagonals of the polygon.
     * @param filename The output SVG file name.
     */
    static void draw(const Polygon &polygon, const vector<Diagonal> &diagonals, const string &filename)
    {
        ofstream svg(filename.c_str());
        if (!svg)
        {
            cerr << "Error opening SVG file for writing." << endl;
            return;
        }

        int svgWidth = 840;
        int svgHeight = 640;

        // Start SVG document.
        svg << "<svg width=\"" << svgWidth << "px\" height=\"" << svgHeight << "px\" "
            << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";

        // Draw each diagonal with y-coordinate corrected.
        for (const auto &d : diagonals)
        {
            // Choose color based on the diagonal type.
            string color = (d.type == "D1") ? "blue" : "brown";
            svg << "<line x1=\"" << d.u.x << "\" y1=\"" << (svgHeight - d.u.y)
                << "\" x2=\"" << d.v.x << "\" y2=\"" << (svgHeight - d.v.y)
                << "\" stroke=\"" << color << "\" stroke-width=\"1\" />\n";
        }

        // Draw the polygon edges using the original vertex order with corrected y coordinates.
        const vector<Vertex> &vertices = polygon.getVertices();
        svg << "<polygon points=\"";
        for (const auto &v : vertices)
            svg << v.x << "," << (svgHeight - v.y) << " ";
        svg << "\" style=\"fill:none;stroke:black;stroke-width:2\" />\n";

        // Draw the vertices as small circles with corrected y coordinates.
        for (const auto &v : vertices)
        {
            svg << "<circle cx=\"" << v.x << "\" cy=\"" << (svgHeight - v.y)
                << "\" r=\"3\" fill=\"black\" />\n";
        }

        // End SVG document.
        svg << "</svg>\n";
        svg.close();
    }
};

/**
 * @brief Main entry point of the program.
 *
 * Reads a polygon from a file, performs triangulation, and outputs an SVG.
 * If the polygon is a triangle, no internal diagonals are drawn.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line arguments (expects the polygon file name as argument).
 * @return 0 on success, 1 on error.
 */
int main(int argc, char *argv[])
{
    // Check command-line arguments.
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <polygon_file.txt>" << endl;
        return 1;
    }
    string filename = argv[1];

    // Create and load the polygon from file.
    Polygon polygon;
    if (!polygon.loadFromFile(filename))
        return 1;

    // If the polygon is a triangle, output it without internal diagonals.
    if (polygon.size() == 3)
    {
        int svgWidth = 840, svgHeight = 640;
        ofstream svg("monopolyTr.svg");
        if (!svg)
        {
            cerr << "Error opening SVG file for writing." << endl;
            return 1;
        }
        svg << "<svg width=\"" << svgWidth << "px\" height=\"" << svgHeight << "px\" "
            << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";
        const vector<Vertex> &vertices = polygon.getVertices();
        svg << "<polygon points=\"";
        for (const auto &v : vertices)
            svg << v.x << "," << (svgHeight - v.y) << " ";
        svg << "\" style=\"fill:none;stroke:black;stroke-width:2\" />\n";
        for (const auto &v : vertices)
            svg << "<circle cx=\"" << v.x << "\" cy=\"" << (svgHeight - v.y)
                << "\" r=\"3\" fill=\"black\" />\n";
        svg << "</svg>\n";
        svg.close();
        cout << "Triangle input; no internal diagonals. Output written to monopolyTr.svg" << endl;
        return 0;
    }

    // For polygons with more than 3 vertices:
    // 1. Assign chains based on x-monotonicity.
    polygon.assignChains();
    // 2. Perform triangulation.
    Triangulator triangulator(polygon);
    vector<Diagonal> diagonals = triangulator.triangulate();

    // 3. Draw the polygon and its diagonals into an SVG file.
    SVGDrawer::draw(polygon, diagonals, "monopolyTr.svg");

    cout << "Triangulation complete. Output written to monopolyTr.svg" << endl;
    return 0;
}
