/*
    Vanshita Garg (19CS10064)
    Ashutosh Kumar Singh (19CS30008)
    Computational Geometry - Homework Set 1
    Question 2 - Point Inclusion in Polygon Problem
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
using namespace std;

const double EPS = 1e-6;

struct Point {
    double x, y;
    
    Point() {}
    Point(double px, double py) : x(px), y(py) {}
    Point(const Point &p) : x(p.x), y(p.y) {}

    friend ostream &operator<<(ostream &os, const Point &p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

// Orientation of the point r w.r.t the line p --> q
// 1 - counter-clockwise, -1 - clockwise, 0 - collinear 
int side(const Point &p, const Point &q, const Point &r) {
    double val = (p.x - r.x) * (q.y - r.y) - (p.y - r.y) * (q.x - r.x);
    if(fabs(val) < EPS)
        return 0;
    return ((val > 0) ? 1 : -1);
}

double angle(const Point &p, const Point &q) {
    double ang = atan2(q.y - p.y, q.x - p.x);
    if(ang < 0)
        ang += 2 * M_PI;
    return ang;
}

// Finds the first element greater than a value in a sorted array using binary search
int find_greater(const vector<Point> &pts, Point &orig, int l, int r, double query_ang) {
    int low = l, high = r, mid, ans = r + 1;
    while(low <= high) {
        mid = (low + high) / 2;
        if(angle(orig, pts[mid]) > query_ang) {
            ans = mid;
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return ans;
}

struct Polygon {
    int n;
    vector<Point> pts;
    Point orig;
    int ind_rot;

    Polygon() {}
    Polygon(const vector<Point> &p) : n((int)p.size()), pts(p) {}

    void preprocess() {
        // Set the origin
        orig.x = (pts[0].x + pts[1].x + pts[2].x) / 3;
        orig.y = (pts[0].y + pts[1].y + pts[2].y) / 3;

        // Calculate the point of rotation
        int low = 0, high = n - 1, mid;
        double ang_0 = angle(orig, pts[0]);
        ind_rot = n;
        while(low <= high) {
            mid = (low + high) / 2;
            if(angle(orig, pts[mid]) < ang_0) {
                ind_rot = mid;
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }
    }

    // 1 - inside, -1 - outside, 0 - on boundary
    int pointInPoly(const Point &p) {
        double query_ang = angle(orig, p);
        int ind_l = find_greater(pts, orig, 0, ind_rot - 1, query_ang);
        int ind_r;

        int v1, v2;
        if(ind_rot == n || ind_l > 0) {     // Left partition
            v2 = ind_l % n;
        }
        else {      // Right partition
            ind_r = find_greater(pts, orig, ind_rot, n - 1, query_ang);
            v2 = ind_r % n;
        }
        v1 = (v2 - 1 + n) % n;
        return side(pts[v1], pts[v2], p);   // check orientation to report inside/outside/on the boundary
    }

    friend ostream &operator<<(ostream &os, const Polygon &poly) {
        for(int i = 0; i < poly.n; i++) {
            os << poly.pts[i] << " ";
        }
        return os;
    }
};

// Prints the header of an SVG to a file
void header(ofstream &file) {
    file << "<?xml version=\"1.0\" standalone=\"no\" ?>" << endl;
    file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
}

// Prints the metadata for an SVG to a file
void startSVG(ofstream &file, int width, int height) {
    file << "<svg width=\"" << width << "px\" height = \"" << height << "px\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << endl;
}

// Marks the end of the SVG
void endSVG(ofstream &file) {
    file << "</svg>" << endl;
}

// Displays a point on the SVG file
void displayPoint(ofstream &file, const Point &p, string col) {
    file << "\t<circle cx=\"" << p.x << "\" cy=\"" << p.y << "\" r=\"2\" stroke=\"" << col << "\" stroke-width=\"2\" fill=\"" << col <<"\"/>" << endl;
}

// Displays a polygon on the SVG file
void displayPolygon(ofstream &file, const Polygon &poly) {
    for(int i = 0; i < poly.n; i++) {
        file << "\t<circle cx=\"" << poly.pts[i].x << "\" cy=\"" << poly.pts[i].y << "\" r=\"2\" stroke=\"black\" stroke-width=\"2\"/>" << endl;
    }
    for(int i = 0; i < poly.n; i++) {
        file << "\t<line x1=\"" << poly.pts[i].x << "\" y1=\"" << poly.pts[i].y << "\" x2=\"" << poly.pts[(i + 1) % poly.n].x << "\" y2=\"" << poly.pts[(i + 1) % poly.n].y << "\" stroke=\"black\" stroke-width=\"1\"/>" << endl;
    }
}

// Generates a random polygon by taking n random points in counter clockwise order on a circle
Polygon genRandomPoly(int n) {
    Point center(400, 400);
    vector<Point> pts(n);
    random_device rd;
    mt19937 gen(rd());
    set<double> theta;
    uniform_real_distribution<double> dis(0, 1);
    while((int)theta.size() < n) {
        theta.insert(dis(gen) * 2 * M_PI);
    }
    double r = dis(gen) * 100 + 250;
    int i = 0;
    for(auto it = theta.begin(); it != theta.end(); it++, i++) {
        pts[i].x = center.x + r * cos(*it);
        pts[i].y = center.y + r * sin(*it);
    }
    return Polygon(pts);
}

int main() {
    int n;
    cout << fixed << setprecision(8);
    cout << "Enter the number of sides for the polygon: ";
    cin >> n;
    Polygon poly = genRandomPoly(n);
    poly.preprocess();

    cout << "\nThe vertices of the random polygon created are: " << endl;
    cout << poly << endl;

    ofstream file("test.svg");
    header(file);
    startSVG(file, 800, 800);
    displayPolygon(file, poly);

    int numPoints;
    cout << "\nEnter the number of points that you want to check: ";
    cin >> numPoints;
    cout << "\nEnter the coordinates of the points one by one (space separated doubles between 0 and 800)" << endl;
    for(int i = 0; i < numPoints; i++) {
        double x, y;
        cout << "\nPoint " << i + 1 << ": "; 
        cin >> x >> y;
        Point p(x, y);
        int res = poly.pointInPoly(p);
        if(res == 1) {
            cout << "Inside" << endl;
            displayPoint(file, p, "red");
        }
        else if(res == -1) {
            cout << "Outside" << endl;
            displayPoint(file, p, "blue");
        }
        else {
            cout << "On the boundary" << endl;
            displayPoint(file, p, "green");
        }
    }
    endSVG(file);

    cout << "\nThe image is available in test.svg" << endl;
    cout << "The vertices and edges of the polygon are black, points inside are red, points outside are blue, points on the boundary are green" << endl;
}